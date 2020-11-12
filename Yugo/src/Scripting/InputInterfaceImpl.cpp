#include "pch.h"
#include "InputInterfaceImpl.h"
#include "Input/UserInput.h"
#include "Scene/MouseRay.h"

namespace Yugo
{

	bool InputInterfaceImpl::IsKeyboardkeyPressed(KeyCode key)
	{
		return UserInput::IsKeyboardKeyPressed(key);
	}
	bool InputInterfaceImpl::IsMouseButtonPressed(MouseCode button)
	{
		return UserInput::IsMouseButtonPressed(button);
	}
	bool InputInterfaceImpl::IsMouseHoveringRect(TransformComponent& transform)
	{
		return MouseRay::CheckCollisionWithSprite(transform);
	}

}