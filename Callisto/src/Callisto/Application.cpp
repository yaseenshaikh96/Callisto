#include "CallistoPCH.h"

#include "Application.h"
#include "GLFW/glfw3.h"

namespace Callisto
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispacther dispatcher{ e };
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	
		CALLISTO_CORE_TRACE("e: {0}", e);
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

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}


}