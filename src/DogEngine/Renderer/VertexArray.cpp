#include "gepch.h"
#include"VertexArray.h"
#include "Renderer.h"
#include "DogEngine/Platform/OpenGL/OpenGLVertexArray.h"
namespace DogEngine {
	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL:return CreateRef<OpenGLVertexArray>();
		}
		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
