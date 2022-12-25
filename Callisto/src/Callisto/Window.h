#pragma once

#include "CallistoPCH.h"

#include <Callisto/Core.h>
#include <Callisto/Events/Event.h>

namespace Callisto
{
	// holds a Window's properties
	class CALLISTO_API WindowProps
	{
	public:
		WindowProps(const std::string& title = "Callisto",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title{ title }, Width{ width }, Height{ height }
		{}

		std::string Title;
		unsigned int Width;
		unsigned int Height;
	};

	// Interface to represent a destop based window
	class CALLISTO_API Window
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		
		virtual void* GetNativeWindow() const = 0;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
	
		// Window Attributes
		virtual void SetEventCallBack(const EventCallBackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// implementated in platform specific Window's implementation
		static Window* Create(const WindowProps& props = WindowProps{});
	};
}