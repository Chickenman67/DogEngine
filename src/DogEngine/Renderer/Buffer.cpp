#include "gepch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "DogEngine/Platform/OpenGL/OpenGLBuffer.h"

namespace DogEngine {					
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL:return CreateRef<OpenGLVertexBuffer>(vertices,size);
		}
		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL:return CreateRef<OpenGLIndexBuffer>(indices, size);
		}
		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
