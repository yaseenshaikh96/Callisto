#include "CallistoPCH.h"

#include "Callisto/Core/Input.h"
#include "Callisto/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Callisto
{

	bool Input::IsKeyPressed(Key key)
	{
		return Input::IsKeyPressed((int)key);
	}
	bool Input::IsMouseButtonPressed(MouseButton mouseButton)
	{
		return Input::IsMouseButtonPressed((int)mouseButton);
	}
	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state{ glfwGetMouseButton(window, button) };
		return state == GLFW_PRESS;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return std::pair<float, float>((float)x, (float)y);
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}
