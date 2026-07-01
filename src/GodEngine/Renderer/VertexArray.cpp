#include "gepch.h"
#include"VertexArray.h"
#include "Renderer.h"
#include "GodEngine/Platform/OpenGL/OpenGLVertexArray.h"
namespace GodEngine {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::None: GE_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
		case RendererAPI::OpenGL:return new OpenGLVertexArray();
		}
		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}