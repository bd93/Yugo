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

	bool UserInput::IsMouseHoveringRect(const glm::vec2& rectPos, const glm::vec2& rectSize, const glm::vec2& mousePos)
	{
		float edgeY1 = rectPos.y;
		float edgeY2 = rectPos.y - rectSize.y;
		float edgeX1 = rectPos.x;
		float edgeX2 = rectPos.x + rectSize.x;

		if (mousePos.x > edgeX1 && mousePos.x < edgeX2 && std::abs(mousePos.y) > std::abs(edgeY1) && glm::abs(mousePos.y) < glm::abs(edgeY2))
			return true;
		else
			return false;
	}

	std::pair<float, float> UserInput::GetMousePosition()
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(Window::GetGLFWwindow(), &mousePosX, &mousePosY);

		return { (float)mousePosX, (float)mousePosY };
	}

}