#pragma once
#include "DogEngine/Renderer/Renderer.h"
namespace DogEngine {
	class OpenGLRendererAPI :public RendererAPI{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void Init() override;
		virtual void DrawIndexed(const Ref<VertexArray>& VertexArray) override;
	};
}
