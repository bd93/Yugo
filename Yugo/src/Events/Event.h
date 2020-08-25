#pragma once
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

namespace Yugo
{

	enum class EventType
	{
		None = 0,
		KeyboardKeyPress, KeyboardKeyRelease, MouseButtonPress, MouseButtonRelease, MouseScroll, MouseMove,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
		ImportAsset
	};

	class Event
	{
	public:
		virtual ~Event() {};
		virtual EventType GetEventType() const = 0;
	};

}