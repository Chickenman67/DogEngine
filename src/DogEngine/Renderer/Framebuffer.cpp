#include "gepch.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "DogEngine/Platform/OpenGL/OpenGLFramebuffer.h"

namespace DogEngine {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: GE_CORE_ASSERT(false, "RendererAPI::None is not supported");
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}
		GE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}
