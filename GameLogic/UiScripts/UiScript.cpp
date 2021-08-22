#include "pch.h"
#include "UiScript.h"
#include "EngineFuncs.h"

namespace GameLogic
{
	Widget* UiScript::CreateWidget(Widget::Type type, Widget* parent)
	{
		return EngineFuncs::GetUiEngineFuncs().CreateWidget(type, parent);
	}
	Widget* UiScript::GetRoot()
	{
		return EngineFuncs::GetUiEngineFuncs().GetUserInterfaceRoot();
	}
}