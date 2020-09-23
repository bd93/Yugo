#pragma once
#include "Core/Common.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Scene/Components.h"

#include <glm/glm.hpp>


namespace Yugo
{

	class UserInput
	{
	public:
		static bool IsKeyboardKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		//static bool IsMouseHoveringRect(TransformComponent& transform, const glm::vec2& mousePos);
		static std::pair<float, float> GetMousePosition();

	private:
		static float s_FramebufferWidth;
		static float s_FramebufferHeight;
	};

}