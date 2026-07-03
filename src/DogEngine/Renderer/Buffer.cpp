#include "gepch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "DogEngine/Platform/OpenGL/OpenGLBuffer.h"

namespace DogEngine {					
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL:return new OpenGLVertexBuffer(vertices,size);
		}
		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* incdices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::API::OpenGL:return new OpenGLIndexBuffer(incdices, size);
		}
		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
