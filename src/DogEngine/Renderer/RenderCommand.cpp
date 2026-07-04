#include "gepch.h"
#include "RenderCommand.h"
#include "DogEngine/Platform/OpenGL/OpenGLRendererAPI.h"
namespace DogEngine {
	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}
