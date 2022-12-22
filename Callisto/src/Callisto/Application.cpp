#include "CallistoPCH.h"

#include "Application.h"
#include "Events/ApplicationEvent.h"

#include "GLFW/glfw3.h"

namespace Callisto
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}


	Application::~Application()
	{

	}


	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.4, 0.01, 0.5, 1.0); // GOOD PURPLE
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

}