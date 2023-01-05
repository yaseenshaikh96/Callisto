#include "CallistoPCH.h"
#include "OpenGLContext.h"


#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Callisto
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CALLISTO_CORE_ASSERT(windowHandle, "Window Handle is null");
	}

	void OpenGLContext::Init()
	{
		CALLISTO_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CALLISTO_CORE_ASSERT(status, "Failed to initialize Glad!");

		CALLISTO_CORE_INFO("OpenGL Info: ");
		CALLISTO_CORE_INFO("	vendor: {0}", (char*)(glGetString(GL_VENDOR)));
		CALLISTO_CORE_INFO("	Renderer: {0}", (char*)(glGetString(GL_RENDERER)));
		CALLISTO_CORE_INFO("	Version: {0}", (char*)(glGetString(GL_VERSION)));
		
		#ifdef CALLISTO_ENABLE_ASSERTS
		int minorVersion;
		int majorVersion;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
			CALLISTO_CORE_ASSERT(majorVersion >= 3 || (majorVersion == 3 && minorVersion >= 3), "OpenGL version 3.3 minimum required!");
		#endif // CALLISTO_ENABLE_ASSERTS


	}
	void OpenGLContext::SwapBuffers()
	{
		CALLISTO_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}