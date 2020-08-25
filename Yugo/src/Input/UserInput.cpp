#include "pch.h"
#include "UserInput.h"
#include "Core/Window.h"


namespace Yugo
{


	bool UserInput::IsKeyboardKeyPressed(KeyCode key)
	{
		auto state = glfwGetKey(Window::GetGLFWwindow(), static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool UserInput::IsMouseButtonPressed(MouseCode button)
	{
		auto state = glfwGetMouseButton(Window::GetGLFWwindow(), static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> UserInput::GetMousePosition()
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(Window::GetGLFWwindow(), &mousePosX, &mousePosY);

		return { (float)mousePosX, (float)mousePosY };
	}

}