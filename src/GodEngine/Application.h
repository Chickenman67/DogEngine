#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "GodEngine/ImGui/ImGuiLayer.h"
#include "GodEngine/Renderer/shader.h"
#include "GodEngine/Renderer/VertexArray.h"
#include <GodEngine/Renderer/OrthoCamera.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

namespace GodEngine {
	class GODENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLay(Layer* layer);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		
		std::shared_ptr<shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		
		std::shared_ptr<shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthoCamera m_Camera;
	private:
		static Application* s_Instance;
	};
	
	Application* CreateApplication();
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
