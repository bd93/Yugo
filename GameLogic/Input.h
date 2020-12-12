#pragma once
#include "Interfaces/InputInterface.h"



namespace GameLogic
{

	class Input
	{
	public:
		static bool IsKeyboardkeyPressed(Yugo::KeyCode key);
		static bool IsMouseButtonPressed(Yugo::MouseCode button);
		static bool IsMouseHoveringRect(TransformComponent& transform);
		static std::pair<float, float> GetMousePosition();
		static std::pair<int, int> GetScreenDimension();

		static void SetInterface(InputInterface* inputInterface);

	private:
		static InputInterface* s_InputInterface;
	};

}