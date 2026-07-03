#include "gepch.h"
#include "DogEngine.h"
#include "imgui/imgui.h"
#include "DogEngine/Events/KeyEvent.h"
#include "DogEngine/Events/ApplicationEvent.h"


class ExampleLayer : public DogEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, .9f, -.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(DogEngine::VertexArray::Create());


		float vertices[3 * 7] = {
			-.5f,-.5f,0.0f, 1.0f,0.0f,0.0f,1.0f,
			.5f,-.5f,.0f,  0.0f,1.0f,0.0f,1.0f,
			.0f,.5f,.0f,   0.0f,0.0f,1.0f,1.0f
		};
		std::shared_ptr<DogEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(DogEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		DogEngine::BufferLayout layout = {
			{"a_Position", DogEngine::ShaderDataType::Float3},
			{"a_Color", DogEngine::ShaderDataType::Float4},

		};
		vertexBuffer->SetLayout(layout);



		m_VertexArray->AddVertexBuffer(vertexBuffer);



		std::shared_ptr<DogEngine::IndexBuffer> indexBuffer;

		unsigned int indices[3] = { 0, 1, 2 };
		indexBuffer.reset(DogEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(DogEngine::VertexArray::Create());
		float sqrvertices[3 * 4] = {
			-.75f,-.75f,0.0f,
			 .75f,-.75f,.0f,
			 .75f, .75f,.0f,
			-.75f, .75f,.0f
		};
		std::shared_ptr<DogEngine::VertexBuffer> squareVB;
		squareVB.reset(DogEngine::VertexBuffer::Create(sqrvertices, sizeof(sqrvertices)));



		squareVB->SetLayout({
			{"a_Position", DogEngine::ShaderDataType::Float3}


			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int sqrindices[6] = { 0, 1, 2 ,2,3,0 };
		std::shared_ptr<DogEngine::IndexBuffer> squareIB;
		squareIB.reset(DogEngine::IndexBuffer::Create(sqrindices, sizeof(sqrindices) / sizeof(uint32_t)));

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
		m_Shader.reset(new DogEngine::shader(vertexSrc, fragmentSrc));


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
		m_BlueShader.reset(new DogEngine::shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate() override {

		if (DogEngine::Input::IsKeyPressed(GE_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (DogEngine::Input::IsKeyPressed(GE_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		if (DogEngine::Input::IsKeyPressed(GE_KEY_UP)) {
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (DogEngine::Input::IsKeyPressed(GE_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		if (DogEngine::Input::IsKeyPressed(GE_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed;
		if (DogEngine::Input::IsKeyPressed(GE_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed;

		DogEngine::RenderCommand::SetClearColor({ .1f, .1f, .1f, 1 });
		DogEngine::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		DogEngine::Renderer::BeginScene(m_Camera);


		DogEngine::Renderer::Submit(m_SquareVA, m_BlueShader);
		DogEngine::Renderer::Submit(m_VertexArray, m_Shader);
		DogEngine::Renderer::EndScene();
}

	virtual void OnImGuiRender() override {
		
	}
	
	void OnEvent(DogEngine::Event& event) override {
		DogEngine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<DogEngine::KeyPressedEvent>([](DogEngine::KeyPressedEvent& e) {
			if (e.GetKeyCode() == GE_KEY_F11) {
				auto& window = DogEngine::Application::Get().GetWindow();
				window.SetFullscreen(!window.IsFullscreen());
				return true;
			}
			return false;
			});
		dispatcher.Dispatch<DogEngine::WindowResizeEvent>([this](DogEngine::WindowResizeEvent& e) {
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
		std::shared_ptr<DogEngine::shader> m_Shader;
		std::shared_ptr<DogEngine::VertexArray> m_VertexArray;

		std::shared_ptr<DogEngine::shader> m_BlueShader;
		std::shared_ptr<DogEngine::VertexArray> m_SquareVA;

		DogEngine::OrthoCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 0.1f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 2.1f;
};

class Sandbox : public DogEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};


DogEngine::Application* DogEngine::CreateApplication() {
	return new Sandbox();
}
