#pragma once
#include "Core/Common.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

#include <glm/glm.hpp>


namespace Yugo
{

	class UserInput
	{
	public:
		static bool IsKeyboardKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static bool IsMouseHoveringRect(const glm::vec2& rectPos, const glm::vec2& rectSize, const glm::vec2& mousePos);
		static std::pair<float, float> GetMousePosition();
	};

}