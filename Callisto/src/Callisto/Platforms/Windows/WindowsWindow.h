#pragma once

#include <Callisto/Core/Window.h>
#include <Callisto/Renderer/GraphicsContext.h>

#include <GLFW/glfw3.h>

namespace Callisto
{
	class WindowsWindow: public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

		inline virtual void* GetNativeWindow() const override { return (void*)m_Window; }

		void OnUpdate() override;
		
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window Attributes
		inline void SetEventCallBack(const EventCallBackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		static uint32_t s_GLFWWindowCount;
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		// Holds a MSWindows Window's property along with a callback
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallBackFn EventCallback;
		};
		WindowData m_Data;
	};
}


