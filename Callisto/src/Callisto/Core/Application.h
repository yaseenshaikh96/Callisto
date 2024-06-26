#pragma once

#include "Callisto/Core/Window.h"
#include "Callisto/Core/Core.h"
#include "Callisto/Core/LayerStack.h"
#include "Callisto/Events/ApplicationEvent.h"
		 
#include "Callisto/Core/TimeStep.h"
		 
#include "Callisto/Imgui/ImguiLayer.h"
		 
#include "Callisto/Renderer/Shader.h"
#include "Callisto/Renderer/Buffer.h"
#include "Callisto/Renderer/VertexArray.h"
		 
#include "Callisto/Renderer/Camera.h"


namespace Callisto
{
	// This object represents the entire application
	class Application
	{
	public:
		Application(const std::string& name = "Callisto App");
		virtual ~Application();
	public:
		void Run(); 
		void OnEvent(Event& e);

		void Close() { m_Running = false; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		Callisto::Ref<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// define in client
	Application* CreateApplication();

}

