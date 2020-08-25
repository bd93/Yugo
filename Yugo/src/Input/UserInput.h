#pragma once
#include "Core/Common.h"
#include "KeyCodes.h"
#include "MouseCodes.h"


namespace Yugo
{

	struct UserInput
	{
		static bool IsKeyboardKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
	};



}