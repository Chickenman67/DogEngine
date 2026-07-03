#include "gepch.h"
#include "RenderCommand.h"
#include "DogEngine/Platform/OpenGL/OpenGLRendererAPI.h"
namespace DogEngine {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}