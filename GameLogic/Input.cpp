#include "pch.h"
#include "Input.h"

#define SCRIPT_API __declspec(dllexport)


namespace GameLogic
{

	InputInterface* Input::s_InputInterface = nullptr;

	bool Input::IsKeyboardkeyPressed(Yugo::KeyCode key)
	{
		return s_InputInterface->IsKeyboardkeyPressed(key);
	}

	bool Input::IsMouseButtonPressed(Yugo::MouseCode button)
	{
		return s_InputInterface->IsMouseButtonPressed(button);
	}

	bool Input::IsMouseHoveringRect(TransformComponent& transform)
	{
		return s_InputInterface->IsMouseHoveringRect(transform);
	}

	void Input::SetInterface(InputInterface* inputInterface)
	{
		s_InputInterface = inputInterface;
	}

	extern "C"
	{
		SCRIPT_API void SetInputInterface(InputInterface* inputInterface)
		{
			Input::SetInterface(inputInterface);
		}
	}

}