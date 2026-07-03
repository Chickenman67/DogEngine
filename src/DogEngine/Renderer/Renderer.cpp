#include "gepch.h"
#include "Renderer.h"

namespace DogEngine {
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	void Renderer::BeginScene(const OrthoCamera& camera)
	{
		m_SceneData->ViewProjecttionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<shader>& Shader)
	{
		Shader->Bind();
		Shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjecttionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
