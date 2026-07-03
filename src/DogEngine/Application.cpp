#include "gepch.h"
#include "Application.h"

#include "Log.h"
#include <glad/glad.h>

#include <DogEngine/Renderer/Renderer.h>


namespace DogEngine {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;

	
	Application::Application() 
	{
		GE_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer();
		PushOverLay(m_ImGuiLayer);
		
	}
	Application	::~Application() {

	}
	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverLay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
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
		
		

		
		while (m_Running) {

			

			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
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
