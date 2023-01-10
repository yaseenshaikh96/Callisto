#pragma once

#include "Callisto/Core/Core.h"

namespace Callisto
{
	class CALLISTO_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}