#pragma once
#include "RenderCommand.h"
#include "OrthoCamera.h"
#include "shader.h"
namespace GodEngine {
	
	class Renderer{

	public:
		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<shader>& Shader);
		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData{
			glm::mat4 ViewProjecttionMatrix;
		};
		static SceneData* m_SceneData;
	};
}
