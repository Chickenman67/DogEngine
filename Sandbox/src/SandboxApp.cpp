#include "gepch.h"
#include "DogEngine.h"
#include "imgui/imgui.h"

#include "DogEngine/Renderer/Renderer2D.h"
#include "DogEngine/Renderer/OrthographicCameraController.h"
#include "DogEngine/Renderer/Texture.h"
#include "DogEngine/Renderer/Framebuffer.h"
#include "DogEngine/Debug/Instrumentor.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#define GL_RGBA8 0x8058
#define GL_DEPTH24_STENCIL8 0x88F0

static DogEngine::Ref<DogEngine::Texture2D> CreateCheckerTexture(uint32_t size)
{
	auto texture = DogEngine::Texture2D::Create(size, size);
	uint32_t* data = new uint32_t[size * size];
	for (uint32_t y = 0; y < size; y++)
		for (uint32_t x = 0; x < size; x++)
		{
			bool light = ((x / 8) + (y / 8)) % 2 == 0;
			uint8_t v = light ? 220 : 40;
			data[y * size + x] = (255 << 24) | (v << 16) | (v << 8) | v;
		}
	texture->SetData(data, size * size * 4);
	delete[] data;
	return texture;
}

static DogEngine::Ref<DogEngine::Texture2D> CreateColorBarsTexture()
{
	const uint32_t w = 256, h = 64;
	auto texture = DogEngine::Texture2D::Create(w, h);
	uint32_t* data = new uint32_t[w * h];
	struct { uint8_t r, g, b; } colors[] = {
		{255,0,0},{255,165,0},{255,255,0},{0,255,0},{0,0,255},{75,0,130},{238,130,238}
	};
	uint32_t barW = w / 7;
	for (uint32_t y = 0; y < h; y++)
		for (uint32_t x = 0; x < w; x++)
		{
			int ci = x / barW;
			if (ci > 6) ci = 6;
			auto& c = colors[ci];
			data[y * w + x] = (255 << 24) | (c.b << 16) | (c.g << 8) | c.r;
		}
	texture->SetData(data, w * h * 4);
	delete[] data;
	return texture;
}

class Sandbox2DLayer : public DogEngine::Layer {
public:
	Sandbox2DLayer()
		: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
	{
		DogEngine::Renderer2D::Init();

		DogEngine::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		fbSpec.Attachments = { GL_RGBA8, GL_DEPTH24_STENCIL8 };
		m_Framebuffer = DogEngine::Framebuffer::Create(fbSpec);

		m_CheckTex = CreateCheckerTexture(128);
		m_BarsTex  = CreateColorBarsTexture();

		// Start zoomed out far enough to see all demo content
		m_CameraController.SetZoomLevel(7.0f);
	}

	~Sandbox2DLayer() { DogEngine::Renderer2D::Shutdown(); }

	void OnUpdate(DogEngine::Timestep ts) override
	{
		PROFILE_FUNCTION();
		m_Time += ts;
		m_DT = ts;
		m_CameraController.OnUpdate(ts);

		auto& spec = m_Framebuffer->GetSpecification();
		if (m_ViewSize.x > 0 && m_ViewSize.y > 0 &&
			(spec.Width != (uint32_t)m_ViewSize.x || spec.Height != (uint32_t)m_ViewSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
			m_CameraController.OnResize(m_ViewSize.x, m_ViewSize.y);
		}

		m_Framebuffer->Bind();
		DogEngine::RenderCommand::SetClearColor({ 0.12f, 0.12f, 0.12f, 1.0f });
		DogEngine::RenderCommand::Clear();
		DogEngine::Renderer2D::ResetStats();
		DogEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// ─── 1. Background grid (shows camera pan/zoom) ──────────────
		for (float x = -10; x <= 10; x += 1)
			for (float y = -10; y <= 10; y += 1)
			{
				float l = (((int)(x + 10) % 2) == ((int)(y + 10) % 2)) ? 0.18f : 0.20f;
				DogEngine::Renderer2D::DrawQuad({ x, y }, { 1, 1 }, { l, l, l, 1 });
			}

		// ─── 2. Center: rotating color wheel (8 wedges) ─────────────
		float rotOff = m_Time * 0.4f;
		glm::vec4 cols[8] = {
			{1,0,0,0.85f},{1,0.5f,0,0.85f},{1,1,0,0.85f},{0,1,0,0.85f},
			{0,0.5f,1,0.85f},{0,0,1,0.85f},{0.5f,0,1,0.85f},{1,0,1,0.85f}
		};
		for (int i = 0; i < 8; i++)
			DogEngine::Renderer2D::DrawRotatedQuad({ 0, 0 }, { 0.6f, 0.6f }, rotOff + i * 45.0f, cols[i]);
		DogEngine::Renderer2D::DrawQuad({ 0, 0 }, { 0.15f, 0.15f }, { 1, 1, 1, 1 });

		// ─── 3. Top‑left: house ─────────────────────────────────────
		float hx = -5.0f, hy = 4.5f;
		DogEngine::Renderer2D::DrawQuad({ hx, hy }, { 2, 1.5f }, { 0.6f, 0.4f, 0.2f, 1 });
		DogEngine::Renderer2D::DrawRotatedQuad({ hx - 0.6f, hy + 1.1f }, { 1.7f, 0.15f }, 35, { 0.8f, 0.2f, 0.1f, 1 });
		DogEngine::Renderer2D::DrawRotatedQuad({ hx + 0.6f, hy + 1.1f }, { 1.7f, 0.15f }, -35, { 0.8f, 0.2f, 0.1f, 1 });
		DogEngine::Renderer2D::DrawQuad({ hx + 0.5f, hy - 0.3f }, { 0.4f, 0.6f }, { 0.3f, 0.2f, 0.1f, 1 });
		DogEngine::Renderer2D::DrawQuad({ hx - 0.5f, hy + 0.1f }, { 0.5f, 0.4f }, { 0.5f, 0.7f, 1, 0.7f });

		// ─── 4. Top‑right: target / bullseye ────────────────────────
		float tx = 5.0f, ty = 4.5f;
		for (int r = 4; r >= 1; r--)
		{
			float s = r * 0.3f;
			bool red = (r % 2 == 0);
			DogEngine::Renderer2D::DrawRotatedQuad({ tx, ty }, { s, s }, 0,
				red ? glm::vec4(0.9f, 0.1f, 0.1f, 0.9f) : glm::vec4(1, 1, 1, 0.9f));
		}

		// ─── 5. Bottom‑left: 3×3 pulsing grid ───────────────────────
		float bx = -5.0f, by = -4.5f;
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
			{
				float vib = 0.6f + 0.4f * glm::sin(m_Time * 0.8f + r * 2.1f + c * 3.7f);
				glm::vec4 color = { vib, vib, vib, 1 };
				if (r == 0) color = { vib, 0.2f, 0.2f, 1 };
				if (r == 1) color = { 0.2f, vib, 0.2f, 1 };
				if (r == 2) color = { 0.2f, 0.2f, vib, 1 };
				DogEngine::Renderer2D::DrawQuad({ bx + c * 0.8f, by - r * 0.8f }, { 0.7f, 0.7f }, color);
			}

		// ─── 6. Bottom‑right: textured quads ────────────────────────
		if (m_CheckTex)
		{
			DogEngine::Renderer2D::DrawQuad({ 4.0f, -4.0f }, { 1.2f, 1.2f }, m_CheckTex, 1);
			DogEngine::Renderer2D::DrawQuad({ 5.5f, -4.0f }, { 1.2f, 1.2f }, m_CheckTex, 3);
			float tR = glm::sin(m_Time * 0.3f) * 180;
			DogEngine::Renderer2D::DrawRotatedQuad({ 6.5f, -4.0f }, { 1.2f, 1.2f }, tR, m_CheckTex, 2);
		}
		if (m_BarsTex)
			DogEngine::Renderer2D::DrawQuad({ 5.25f, -5.5f }, { 2.2f, 0.6f }, m_BarsTex, 1);

		// ─── 7. Sparkle star field ──────────────────────────────────
		for (int i = 0; i < 40; i++)
		{
			float sx = -2.0f + (i % 8) * 0.55f;
			float sy = -5.5f + (i / 8) * 0.55f;
			float si = 0.08f + 0.04f * glm::sin(m_Time * 1.5f + i * 1.7f);
			float sp = m_Time * 60.0f + i * 37.0f;
			float blip = 0.5f + 0.5f * glm::sin(m_Time * 2.0f + i);
			DogEngine::Renderer2D::DrawRotatedQuad({ sx, sy }, { si, si * 3 }, sp, { blip, blip * 0.5f, 1 - blip, 0.9f });
			DogEngine::Renderer2D::DrawRotatedQuad({ sx, sy }, { si * 3, si }, sp + 45, { blip, blip * 0.5f, 1 - blip, 0.9f });
		}

		// ─── 8. Cross‑hatch rays ────────────────────────────────────
		for (int i = 0; i < 12; i++)
		{
			float ang = glm::radians(i * 30.0f + m_Time * 20.0f);
			float ex = glm::cos(ang) * 2.5f;
			float ey = glm::sin(ang) * 2.5f;
			DogEngine::Renderer2D::DrawRotatedQuad({ ex, ey }, { 0.03f, 0.5f }, m_Time * 90, { 1, 1, 0, 0.4f });
		}

		// ─── 9. Pulsing ring ────────────────────────────────────────
		float ringPulse = 1.0f + 0.15f * glm::sin(m_Time * 1.2f);
		for (int i = 0; i < 24; i++)
		{
			float ang = glm::radians(i * 15.0f);
			float rx = glm::cos(ang) * 2.8f * ringPulse;
			float ry = glm::sin(ang) * 2.8f * ringPulse;
			DogEngine::Renderer2D::DrawRotatedQuad({ rx, ry }, { 0.15f, 0.15f }, m_Time * 60, { 0.2f, 0.8f, 0.4f, 0.7f });
		}

		// ─── 10. Alpha‑blending overlap demo ────────────────────────
		float arx = -5.8f, ary = -2.5f;
		DogEngine::Renderer2D::DrawQuad({ arx, ary }, { 0.6f, 0.6f }, { 0.2f, 0.7f, 0.3f, 0.5f });
		DogEngine::Renderer2D::DrawQuad({ arx + 0.3f, ary }, { 0.6f, 0.6f }, { 0.7f, 0.2f, 0.3f, 0.5f });
		DogEngine::Renderer2D::DrawQuad({ arx + 0.15f, ary + 0.3f }, { 0.6f, 0.6f }, { 0.3f, 0.3f, 0.7f, 0.5f });

		DogEngine::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Viewport");
		bool hovered = ImGui::IsWindowHovered();
		DogEngine::Application::Get().GetImGuiLayer()->BlockEvents(hovered);
		ImVec2 sz = ImGui::GetContentRegionAvail();
		m_ViewSize = { sz.x, sz.y };
		uint32_t tid = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)(intptr_t)tid, sz, { 0, 1 }, { 1, 0 });
		ImGui::End();

		ImGui::Begin("Renderer Stats");
		auto s = DogEngine::Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", s.DrawCalls);
		ImGui::Text("Quads:      %d", s.QuadCount);
		ImGui::Text("FPS:        %.0f", 1.0f / m_DT);
		ImGui::Text("Frame:      %.2f ms", m_DT * 1000);
		ImGui::End();

		ImGui::Begin("Controls");
		ImGui::Text("WASD        — Move camera");
		ImGui::Text("Scroll      — Zoom in/out");
		ImGui::Text("F11         — Fullscreen");
		ImGui::Separator();
		ImGui::Text("Demo shows:");
		ImGui::BulletText("Color wheel (center, rotating)");
		ImGui::BulletText("House (top-left)");
		ImGui::BulletText("Bullseye target (top-right)");
		ImGui::BulletText("Pulsing RGB grid (bottom-left)");
		ImGui::BulletText("Textured + tiled quads (bottom-right)");
		ImGui::BulletText("Sparkles, rays & pulsing ring");
		ImGui::BulletText("Alpha blending (bottom-center)");
		ImGui::BulletText("Batch renderer (all quads)");
		ImGui::End();
	}

	void OnEvent(DogEngine::Event& e) override { m_CameraController.OnEvent(e); }

private:
	DogEngine::OrthographicCameraController m_CameraController;
	DogEngine::Ref<DogEngine::Framebuffer>   m_Framebuffer;
	DogEngine::Ref<DogEngine::Texture2D>     m_CheckTex;
	DogEngine::Ref<DogEngine::Texture2D>     m_BarsTex;
	glm::vec2 m_ViewSize = { 1280, 720 };
	float     m_Time = 0;
	float     m_DT   = 0.016f;
};

class Sandbox : public DogEngine::Application {
public:
	Sandbox() { PushLayer(DogEngine::CreateScope<Sandbox2DLayer>()); }
};

DogEngine::Application* DogEngine::CreateApplication() { return new Sandbox(); }
