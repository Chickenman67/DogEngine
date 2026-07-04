#include "gepch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DogEngine {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::mat4 ViewProjectionMatrix;

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(nullptr, s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ "a_Position", ShaderDataType::Float3 },
			{ "a_Color", ShaderDataType::Float4 },
			{ "a_TexCoord", ShaderDataType::Float2 },
			{ "a_TexIndex", ShaderDataType::Float },
			{ "a_TilingFactor", ShaderDataType::Float }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Create white 1x1 texture for untextured quads
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Create batching shader
		s_Data.TextureShader = Shader::Create("BatchRenderer2D",
			R"(
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

void main() {
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
)",
			R"(
#version 330 core
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main() {
	int index = int(v_TexIndex);
	vec4 texColor = texture(u_Textures[index], v_TexCoord * v_TilingFactor);
	color = texColor * v_Color;
}
)"
		);
	}

	void Renderer2D::Shutdown() {
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera) {
		s_Data.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		StartNewBatch();
	}

	void Renderer2D::EndScene() {
		Flush();
	}

	void Renderer2D::Flush() {
		if (s_Data.QuadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(dataSize, s_Data.QuadVertexBufferBase);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ViewProjection", s_Data.ViewProjectionMatrix);

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);

		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset() {
		Flush();
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::StartNewBatch() {
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	// ---- Internal Draw Implementation ----

	static uint32_t FindOrAllocateTextureSlot(const Ref<Texture2D>& texture) {
		// Search for existing texture in slots
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			// Compare renderer IDs — works because OpenGLTexture2D exposes GetRendererID
			// but since it's abstract we'll use a different approach
			if (s_Data.TextureSlots[i] == texture)
				return i;
		}

		// Need to allocate new slot
		if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			Renderer2D::FlushAndReset();

		// If we flushed, texture slot 1 is now free
		// Check again in case FlushAndReset was called
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (s_Data.TextureSlots[i] == texture)
				return i;
		}

		uint32_t slot = s_Data.TextureSlotIndex;
		s_Data.TextureSlots[slot] = texture;
		s_Data.TextureSlotIndex++;
		return slot;
	}

	static void DrawQuadInternal(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg = 0.0f, float textureIndex = 0.0f, float tilingFactor = 1.0f) {
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			Renderer2D::FlushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		if (rotationDeg != 0.0f)
			transform = glm::rotate(transform, glm::radians(rotationDeg), { 0.0f, 0.0f, 1.0f });
		transform = glm::scale(transform, { size.x, size.y, 1.0f });

		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = (float)textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = (float)textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = (float)textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = (float)textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	// ---- Public DrawQuad Implementations ----

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuadInternal(position, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		uint32_t slot = FindOrAllocateTextureSlot(texture);
		DrawQuadInternal(position, size, tintColor, 0.0f, (float)slot, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationDeg, const glm::vec4& color) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationDeg, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationDeg, const glm::vec4& color) {
		DrawQuadInternal(position, size, color, rotationDeg);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationDeg, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		uint32_t slot = FindOrAllocateTextureSlot(texture);
		DrawQuadInternal(position, size, tintColor, rotationDeg, (float)slot, tilingFactor);
	}

	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.Stats;
	}

}
