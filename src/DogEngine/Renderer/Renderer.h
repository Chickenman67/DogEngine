#pragma once
#include "RenderCommand.h"
#include "OrthoCamera.h"
#include "DogEngine/Renderer/Shader.h"
namespace DogEngine {
	
	class Renderer{

	public:
		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader);
		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData{
			glm::mat4 ViewProjecttionMatrix;
		};
		static SceneData* m_SceneData;
	};
}
