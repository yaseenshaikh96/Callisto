#include "CallistoPCH.h"
#include "WindowsWindow.h"

#include "Callisto/Events/ApplicationEvent.h"
#include "Callisto/Events/MouseEvent.h"
#include "Callisto/Events/KeyEvent.h"

namespace Callisto
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		CALLISTO_CORE_ERROR("GLFW error ({0}): {1}", error, description);
	}

	// Return MSWindow's window, this function is implemented for each platform
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		CALLISTO_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// terminate glfw on shutdown
			int success = glfwInit();
			CALLISTO_CORE_ASSERT(success, "Could not initialze GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		/*
		*	WindowsWindow.m_Data.callbackFn is a pointer to Application::OnEvent().
		*	we pass &m_Data to glfw and associate it with the current opened window.
		*	
		*	glfw gets all the events from Operating System.
		*	we tell which function to run on which event.
		*	these functions are the lamdas defined below
		*
		*	for every glfwEvent, we make our own corresponding Event obj.
		*	we call m_Data.EventCallbackFn(e) with our Event obj.
		* 
		*	the lamdas only create the events and call m_Data.EventCallbackFn(e)
		*	m_Data.EventCallbackFn(e) is the Application::OnEvent() function.
		* 
		*	And in the Application::OnEvent() we decide what to do with the events, we can log them or do things when these event occur
		*/

		glfwSetWindowSizeCallback(m_Window, 
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				data.Width = width;
				data.Height = height;
				
				WindowResizeEvent e(width, height);
				data.EventCallback(e);
			});

		glfwSetWindowCloseCallback(m_Window,
			[](GLFWwindow* window)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent e{};
				data.EventCallback(e);
			});
		
		glfwSetKeyCallback(m_Window,
			[](GLFWwindow* window, int key, int scancode, int action, int mode)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e{ key, 0 };
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e{ key };
					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e{ key, 1 };
					data.EventCallback(e);
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(m_Window,
			[](GLFWwindow* window, int button, int action, int mode)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e{ button };
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e{ button };
					data.EventCallback(e);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window,
			[](GLFWwindow* window, double offsetX, double offsetY)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				MouseScrollEvent e(static_cast<float>(offsetX), static_cast<float>(offsetY));
				data.EventCallback(e);
			});

		glfwSetCursorPosCallback(m_Window,
			[](GLFWwindow* window, double posX, double posY)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				MouseMovedEvent e(static_cast<float>(posX), static_cast<float>(posY));
				data.EventCallback(e);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}