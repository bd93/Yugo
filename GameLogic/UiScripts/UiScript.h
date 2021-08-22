#pragma once
#include "Script.h"
#include "Yugo.h"

namespace GameLogic
{
	class UiScript : public Script
	{
	protected:
		Widget* CreateWidget(Widget::Type type, Widget* parent);
		Widget* GetRoot();
	};
}