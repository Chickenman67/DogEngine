#include "gepch.h"
#include "GodEngine.h"
#include "imgui/imgui.h"
#include "GodEngine/Events/KeyEvent.h"
#include "GodEngine/Events/ApplicationEvent.h"


class ExampleLayer : public GodEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, .9f, -.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(GodEngine::VertexArray::Create());


		float vertices[3 * 7] = {
			-.5f,-.5f,0.0f, 1.0f,0.0f,0.0f,1.0f,
			.5f,-.5f,.0f,  0.0f,1.0f,0.0f,1.0f,
			.0f,.5f,.0f,   0.0f,0.0f,1.0f,1.0f
		};
		std::shared_ptr<GodEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(GodEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		GodEngine::BufferLayout layout = {
			{"a_Position", GodEngine::ShaderDataType::Float3},
			{"a_Color", GodEngine::ShaderDataType::Float4},

		};
		vertexBuffer->SetLayout(layout);



		m_VertexArray->AddVertexBuffer(vertexBuffer);



		std::shared_ptr<GodEngine::IndexBuffer> indexBuffer;

		unsigned int indices[3] = { 0, 1, 2 };
		indexBuffer.reset(GodEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(GodEngine::VertexArray::Create());
		float sqrvertices[3 * 4] = {
			-.75f,-.75f,0.0f,
			 .75f,-.75f,.0f,
			 .75f, .75f,.0f,
			-.75f, .75f,.0f
		};
		std::shared_ptr<GodEngine::VertexBuffer> squareVB;
		squareVB.reset(GodEngine::VertexBuffer::Create(sqrvertices, sizeof(sqrvertices)));



		squareVB->SetLayout({
			{"a_Position", GodEngine::ShaderDataType::Float3}


			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int sqrindices[6] = { 0, 1, 2 ,2,3,0 };
		std::shared_ptr<GodEngine::IndexBuffer> squareIB;
		squareIB.reset(GodEngine::IndexBuffer::Create(sqrindices, sizeof(sqrindices) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			out vec4 v_Color;

			void main(){
				v_Color = a_Color;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
)";
		std::string fragmentSrc = R"(
			#version 330 core
			layout(location=0) out vec4 color;
			in vec4 v_Color;

			void main(){
				color = v_Color;
			
})";
		m_Shader.reset(new GodEngine::shader(vertexSrc, fragmentSrc));


		std::string blueShaderVertexSrc = R"(
			#version 330 core
			layout(location=0) in vec3 a_Position;
uniform mat4 u_ViewProjection;
			out vec3 v_Position;

			void main(){
				v_Position = a_Position;
				gl_Position = u_ViewProjection*vec4(a_Position, 1.0);
			}
)";
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			layout(location=0) out vec4 color;

			in vec3 v_Position;

			void main(){
				color = vec4(0.2,.3,.8,1.0);
			
})";
		m_BlueShader.reset(new GodEngine::shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate() override {

		if (GodEngine::Input::IsKeyPressed(GE_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (GodEngine::Input::IsKeyPressed(GE_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		if (GodEngine::Input::IsKeyPressed(GE_KEY_UP)) {
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (GodEngine::Input::IsKeyPressed(GE_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		if (GodEngine::Input::IsKeyPressed(GE_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed;
		if (GodEngine::Input::IsKeyPressed(GE_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed;

		GodEngine::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		GodEngine::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		GodEngine::Renderer::BeginScene(m_Camera);


		GodEngine::Renderer::Submit(m_SquareVA, m_BlueShader);
		GodEngine::Renderer::Submit(m_VertexArray, m_Shader);
		GodEngine::Renderer::EndScene();
}

	virtual void OnImGuiRender() override {
		
	}
	
	void OnEvent(GodEngine::Event& event) override {
		GodEngine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<GodEngine::KeyPressedEvent>([](GodEngine::KeyPressedEvent& e) {
			if (e.GetKeyCode() == GE_KEY_F11) {
				auto& window = GodEngine::Application::Get().GetWindow();
				window.SetFullscreen(!window.IsFullscreen());
				return true;
			}
			return false;
			});
		dispatcher.Dispatch<GodEngine::WindowResizeEvent>([this](GodEngine::WindowResizeEvent& e) {
			float aspect = (float)e.GetWidth() / (float)e.GetHeight();
			float orthoSize = 1.0f;
			if (aspect > 1.0f)
				m_Camera.SetProjection(-orthoSize * aspect, orthoSize * aspect, orthoSize, -orthoSize);
			else
				m_Camera.SetProjection(-orthoSize, orthoSize, orthoSize / aspect, -orthoSize / aspect);
			return false;
			});
	}
	
	private:
		std::shared_ptr<GodEngine::shader> m_Shader;
		std::shared_ptr<GodEngine::VertexArray> m_VertexArray;

		std::shared_ptr<GodEngine::shader> m_BlueShader;
		std::shared_ptr<GodEngine::VertexArray> m_SquareVA;

		GodEngine::OrthoCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 0.1f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 2.1f;
};

class Sandbox : public GodEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};


GodEngine::Application* GodEngine::CreateApplication() {
	return new Sandbox();
}
