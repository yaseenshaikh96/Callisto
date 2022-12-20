#pragma once

#include "Core.h"

namespace Callisto
{
	class CALLISTO_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// define in client
	Application* CreateApplication();

}

