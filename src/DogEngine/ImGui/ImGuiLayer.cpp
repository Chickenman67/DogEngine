#include "gepch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"	
#include "backends/imgui_impl_glfw.h"
#include "DogEngine/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace DogEngine{

	static void SetDefaultDockLayout(ImGuiID dockspace_id) {
		// Only apply default layout the first frame
		static bool layoutDone = false;
		if (layoutDone) return;

		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

		// Split into left (viewport) and right (panels)
		ImGuiID dockRight;
		ImGuiID dockMain = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.22f, nullptr, &dockRight);

		// Split right side into Stats (top) and Controls+About (bottom)
		ImGuiID dockStats;
		ImGuiID dockControls = ImGui::DockBuilderSplitNode(dockRight, ImGuiDir_Down, 0.55f, &dockStats, nullptr);

		// Assign windows
		ImGui::DockBuilderDockWindow("Viewport", dockMain);
		ImGui::DockBuilderDockWindow("Renderer Stats", dockStats);
		ImGui::DockBuilderDockWindow("Controls", dockControls);
		ImGui::DockBuilderDockWindow("About DogEngine", dockControls);

		ImGui::DockBuilderFinish(dockspace_id);
		layoutDone = true;
	}

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {
	}
	ImGuiLayer::~ImGuiLayer() {}
	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiLayer::End() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// When the viewport is focused, let keyboard/mouse events pass through
		// to the camera controller instead of being captured by ImGui
		if (m_BlockEvents) {
			io.WantCaptureKeyboard = false;
			io.WantCaptureMouse = false;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::OnImGuiRender() {
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			// Set default layout on first frame
			SetDefaultDockLayout(dockspace_id);
		}
		style.WindowMinSize.x = minWinSizeX;

		ImGui::End();
	}

}
