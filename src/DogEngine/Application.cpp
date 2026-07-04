#include "gepch.h"
#include "Application.h"

#include "Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <DogEngine/Renderer/Renderer.h>
#include <DogEngine/Debug/Instrumentor.h>


namespace DogEngine {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;

	
	Application::Application() 
	{
		GE_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;
		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		RenderCommand::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(Scope<Layer>(m_ImGuiLayer));
		
	}
	Application	::~Application() {

	}
	void Application::PushLayer(Scope<Layer> layer) {
		Layer* raw = layer.get();
		m_LayerStack.PushLayer(std::move(layer));
		raw->OnAttach();
	}
	void Application::PushOverLay(Scope<Layer> layer) {
		Layer* raw = layer.get();
		m_LayerStack.PushOverlay(std::move(layer));
		raw->OnAttach();
	}
	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		GE_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end();it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	
	void Application::Run() {
		PROFILE_SCOPE("Application::Run");

		float lastFrameTime = 0.0f;
		
		while (m_Running) {
			PROFILE_SCOPE("Frame");

			float time = (float)glfwGetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			// Skip rendering while minimized
			if (m_Window->GetWidth() == 0 || m_Window->GetHeight() == 0) {
				m_Window->OnUpdate();
				continue;
			}

			for (Scope<Layer>& layer : m_LayerStack)
				layer->OnUpdate(timestep);
			m_ImGuiLayer->Begin();
			for (Scope<Layer>& layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	
	}
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}
