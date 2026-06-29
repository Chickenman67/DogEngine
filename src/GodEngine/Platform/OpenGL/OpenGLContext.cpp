#include "gepch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
namespace GodEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):m_WindowHandle(windowHandle)
	{
		GE_CORE_ASSERT(windowHandle, "Window Handle is  null")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GE_CORE_ASSERT(gladStatus, "Failed to initialize Glad!");

		GE_CORE_INFO("OpenGL Info:")
		GE_CORE_INFO(" Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		GE_CORE_INFO(" Renderer: {}", (const char*)glGetString(GL_RENDERER));
		GE_CORE_INFO(" Version: {}", (const char*)glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
	
}