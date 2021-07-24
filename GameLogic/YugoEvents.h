#pragma once
#include "Events/Event.h"
#include "Events/KeyboardKeyEvent.h"
#include "Events/MouseButtonEvent.h"

namespace GameLogic
{

	using Event = Yugo::Event;
	using EventType = Yugo::EventType;
	using MouseButtonPress = Yugo::MouseButtonPress;
	using KeyboardKeyPress = Yugo::KeyboardKeyPress;
	using KeyCode = Yugo::KeyCode;
	using MouseCode = Yugo::MouseCode;

}