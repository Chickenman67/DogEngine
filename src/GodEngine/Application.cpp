#include "gepch.h"
#include "Application.h"

#include "Log.h"
#include <glad/glad.h>




namespace GodEngine {

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
		m_VertexArray.reset(VertexArray::Create());
		
		
		float vertices[3 * 7] = {
			-.5f,-.5f,0.0f, 1.0f,0.0f,0.0f,1.0f,
			.5f,-.5f,.0f,  0.0f,1.0f,0.0f,1.0f,
			.0f,.5f,.0f,   0.0f,0.0f,1.0f,1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
			BufferLayout layout = {
				{"a_Position", ShaderDataType::Float3},
				{"a_Color", ShaderDataType::Float4},

			};
			vertexBuffer->SetLayout(layout);
		
		

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		

		std::shared_ptr<IndexBuffer> indexBuffer;

		unsigned int indices[3] = { 0, 1, 2 };
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());
		float sqrvertices[3 * 4] = {
			-.75f,-.75f,0.0f,
			 .75f,-.75f,.0f,
			 .75f, .75f,.0f,
			-.75f, .75f,.0f
		};			
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(sqrvertices, sizeof(sqrvertices)));
		

		
		squareVB->SetLayout({
			{"a_Position", ShaderDataType::Float3}
			

			});
		m_SquareVA->AddVertexBuffer(squareVB);
			
		unsigned int sqrindices[6] = { 0, 1, 2 ,2,3,0};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(sqrindices, sizeof(sqrindices) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;

			out vec4 v_Color;

			void main(){
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
)";
		std::string fragmentSrc = R"(
			#version 330 core
			layout(location=0) out vec4 color;
			in vec4 v_Color;

			void main(){
				color = v_Color;
			
})";
		m_Shader.reset(new shader(vertexSrc, fragmentSrc));	


		std::string blueShaderVertexSrc = R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;

			out vec3 v_Position;

			void main(){
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
)";
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			layout(location=0) out vec4 color;
			in vec3 v_Position;

			void main(){
				color = vec4(0.2,.3,.8,1.0);
			
})";
		m_BlueShader.reset(new shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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

			glClearColor(.1f, .1f, .1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			
			m_BlueShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES,m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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
