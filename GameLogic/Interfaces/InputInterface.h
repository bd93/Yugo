#pragma once
#include "YugoEvents.h"
#include "YugoComponents.h"



namespace GameLogic
{

	class InputInterface
	{
	public:
		virtual bool IsKeyboardkeyPressed(KeyCode key) = 0;
		virtual bool IsMouseButtonPressed(MouseCode button) = 0;
		virtual bool IsMouseHoveringRect(TransformComponent& transform) = 0;
	};

}