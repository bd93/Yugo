#include "pch.h"
#include "Input.h"
#include "EngineFuncs.h"


namespace GameLogic
{

	bool Input::IsKeyboardKeyPressed(KeyCode keyCode)
	{
		return EngineFuncs::GetGameEngineFuncs().IsKeyboardKeyPressed(keyCode);
	}

	bool Input::IsMouseButtonPressed(MouseCode mouseCode)
	{
		return EngineFuncs::GetGameEngineFuncs().IsMouseButtonPressed(mouseCode);
	}

	float const Input::GetMousePosX()
	{
		return EngineFuncs::GetGameEngineFuncs().GetMousePosX();
	}

	float const Input::GetMousePosY()
	{
		return EngineFuncs::GetGameEngineFuncs().GetMousePosY();
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return { GetMousePosX(), GetMousePosY() };
	}

}