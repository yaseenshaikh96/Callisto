#pragma once

#include "Core.h"
#include "Window.h"
#include <Callisto/LayerStack.h>
#include <Callisto/Events/ApplicationEvent.h>

#include <Callisto/Imgui/ImguiLayer.h>

#include "Callisto/Renderer/Shader.h"
#include <Callisto/Renderer/Buffer.h>

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

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	private:
		static Application* s_Instance;
	};

	// define in client
	Application* CreateApplication();

}

