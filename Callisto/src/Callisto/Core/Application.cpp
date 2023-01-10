#include "CallistoPCH.h"
#include "Application.h"

#include "Callisto/Core/Log.h"
#include "Callisto/Renderer/Renderer.h"
#include "Callisto/Core/Input.h"
#include "Callisto/Core/keycodes.h"

#include <GLFW/glfw3.h>


namespace Callisto
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		CALLISTO_PROFILE_FUNCTION();

		CALLISTO_CORE_ASSERT(!s_Instance, "Application already exist!");
		s_Instance = this;

		m_Window = Ref<Window>(Window::Create(WindowProps(name) ));
		m_Window->SetEventCallBack(BIND_EVENT_FN(OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		CALLISTO_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		CALLISTO_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		CALLISTO_PROFILE_FUNCTION();

		EventDispacther dispatcher = e;
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResizeEvent));
	
		//CALLISTO_CORE_TRACE("e: {0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*(--it))->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		CALLISTO_PROFILE_FUNCTION();

		while (m_Running)
		{	
			CALLISTO_PROFILE_SCOPE("Run Loop");

			float time = (float)glfwGetTime();
			TimeStep timeStep(time - m_LastFrameTime);
			m_LastFrameTime = time;

			{
				CALLISTO_PROFILE_SCOPE("LayerStack OnUpdate");
				if (!m_Minimized)
				{
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timeStep);
				}
			}

			m_ImGuiLayer->Begin();
			{
				CALLISTO_PROFILE_SCOPE("ImGuiLayer OnUpdate:");
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		CALLISTO_PROFILE_FUNCTION();

		if(e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

}