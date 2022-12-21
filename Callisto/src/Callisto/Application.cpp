#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Callisto/Log.h"

namespace Callisto
{
	Application::Application()
	{

	}


	Application::~Application()
	{

	}


	void Application::Run()
	{
		WindowResizeEvent e{ 1280, 720 };

		if (e.IsInCategory(EventCategoryApplication))
		{
			CALLISTO_CORE_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			CALLISTO_CORE_WARN(e);
		}

		while (true)
		{
		}
	}

}