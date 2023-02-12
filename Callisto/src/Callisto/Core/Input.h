#pragma once

#include "Callisto/Core/Core.h"
#include "Callisto/Core/KeyCodes.h"
#include "Callisto/Core/MouseButtonCodes.h"

namespace Callisto
{
	class Input
	{
	public:

		static bool IsKeyPressed(Key key);
		static bool IsMouseButtonPressed(MouseButton mouseButton);
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}