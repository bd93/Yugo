#pragma once
#include "Yugo.h"


namespace GameLogic
{

	class Input
	{
	public:
		static bool IsKeyboardKeyPressed(KeyCode keyCode);
		static bool IsMouseButtonPressed(MouseCode mouseCode);
		static float const GetMousePosX();
		static float const GetMousePosY();
		static std::pair<float, float> GetMousePosition();
	};

}