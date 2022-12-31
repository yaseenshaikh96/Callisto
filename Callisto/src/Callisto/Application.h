#pragma once

#include "Core.h"
#include "Window.h"
#include <Callisto/LayerStack.h>
#include <Callisto/Events/ApplicationEvent.h>

#include <Callisto/Imgui/ImguiLayer.h>

#include "Callisto/Renderer/Shader.h"

namespace Callisto
{
	// This object represents the entire application
	class CALLISTO_API Application
	{
	public:
		Application();
		virtual ~Application();
	public:
		void Run(); 
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImguiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

	private:
		static Application* s_Instance;
	};

	// define in client
	Application* CreateApplication();

}

