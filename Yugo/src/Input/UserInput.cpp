#include "pch.h"
#include "UserInput.h"
#include "Core/Window.h"


namespace Yugo
{
	GLFWwindow* s_GLFWwindow = nullptr; // Current glfw window, which can be changed

	bool UserInput::IsKeyboardKeyPressed(KeyCode key)
	{
		auto state = glfwGetKey(s_GLFWwindow, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool UserInput::IsMouseButtonPressed(MouseCode button)
	{
		auto state = glfwGetMouseButton(s_GLFWwindow, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	//bool UserInput::IsMouseHoveringRect(TransformComponent& transform, const glm::vec2& mousePos)
	//{
	//	float edgeY1 = transform.Position.y;
	//	float edgeY2 = transform.Position.y - transform.Scale.y;
	//	float edgeX1 = transform.Position.x;
	//	float edgeX2 = transform.Position.x + transform.Scale.x;

	//	if (mousePos.x > edgeX1 && mousePos.x < edgeX2 && std::abs(mousePos.y) > std::abs(edgeY1) && glm::abs(mousePos.y) < glm::abs(edgeY2))
	//		return true;
	//	else
	//		return false;
	//}

	std::pair<float, float> UserInput::GetMousePosition()
	{
		double mousePosX, mousePosY;
		glfwGetCursorPos(s_GLFWwindow, &mousePosX, &mousePosY);
		return { (float)mousePosX, (float)mousePosY };
	}

	std::pair<int, int> UserInput::GetScreenDimension()
	{
		int screenWidth, screenHeight;
		glfwGetFramebufferSize(s_GLFWwindow, &screenWidth, &screenHeight);
		return { screenWidth, screenHeight };
	}

	void UserInput::SetGLFWwindow(GLFWwindow* glfwWindow)
	{
		s_GLFWwindow = glfwWindow;
	}

}