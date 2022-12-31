#include "CallistoPCH.h"
#include "OpenGLContext.h"


#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Callisto
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle{ windowHandle }
	{
		CALLISTO_CORE_ASSERT(windowHandle, "Window Handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CALLISTO_CORE_ASSERT(status, "Failed to initialize Glad!");

	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}