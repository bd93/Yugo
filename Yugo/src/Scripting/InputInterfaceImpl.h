#pragma once
#include "Interfaces/InputInterface.h"



namespace Yugo
{

	class InputInterfaceImpl : public GameLogic::InputInterface
	{
	public:
		virtual bool IsKeyboardkeyPressed(KeyCode key) override;
		virtual bool IsMouseButtonPressed(MouseCode button) override;
		virtual bool IsMouseHoveringRect(TransformComponent& transform) override;
	};

}