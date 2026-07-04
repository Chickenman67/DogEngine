#include "gepch.h"
#include "DogEngine.h"
#include "imgui/imgui.h"

#include "DogEngine/Renderer/Renderer2D.h"
#include "DogEngine/Renderer/OrthographicCameraController.h"
#include "DogEngine/Renderer/Texture.h"
#include "DogEngine/Debug/Instrumentor.h"

class Sandbox2DLayer : public DogEngine::Layer {
public:
	Sandbox2DLayer()
		: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {

		DogEngine::Renderer2D::Init();
	}

	void OnUpdate(DogEngine::Timestep ts) override {
		PROFILE_FUNCTION();

		m_CameraController.OnUpdate(ts);

		DogEngine::Renderer2D::ResetStats();

		DogEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		DogEngine::RenderCommand::Clear();

		DogEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// Draw colored quads
		DogEngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		DogEngine::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.2f, 0.3f, 0.8f, 1.0f });

		// Draw rotated colored quad
		DogEngine::Renderer2D::DrawRotatedQuad({ 0.0f, -1.0f }, { 0.8f, 0.8f }, 45.0f, { 0.2f, 0.8f, 0.3f, 1.0f });

		// Draw textured quads (using white texture tinted = colored quad through single shader path)
		DogEngine::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.5f });
		DogEngine::Renderer2D::DrawRotatedQuad({ 0.0f, 1.5f }, { 1.2f, 0.8f }, 30.0f, { 1.0f, 0.5f, 0.0f, 0.8f });

		// Draw many quads to exercise batching
		for (float x = -5.0f; x < 5.0f; x += 1.1f) {
			for (float y = -5.0f; y < 5.0f; y += 1.1f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.5f, 0.7f };
				DogEngine::Renderer2D::DrawQuad({ x, y }, { 0.9f, 0.9f }, color);
			}
		}

		DogEngine::Renderer2D::EndScene();
	}

	virtual void OnImGuiRender() override {
		auto stats = DogEngine::Renderer2D::GetStats();
		ImGui::Begin("Renderer2D Stats");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::Begin("Controls");
		ImGui::Text("WASD - Move camera");
		ImGui::Text("Scroll - Zoom in/out");
		ImGui::Text("F11 - Toggle fullscreen");
		ImGui::End();
	}

	void OnEvent(DogEngine::Event& event) override {
		m_CameraController.OnEvent(event);
	}

private:
	DogEngine::OrthographicCameraController m_CameraController;
};

class Sandbox : public DogEngine::Application {
public:
	Sandbox() {
		PushLayer(DogEngine::CreateScope<Sandbox2DLayer>());
	}
	~Sandbox() = default;
};

DogEngine::Application* DogEngine::CreateApplication() {
	return new Sandbox();
}
