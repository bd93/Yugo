#pragma once
#include "Event.h"

namespace Yugo
{

	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

	protected:
		KeyEvent(KeyCode keyCode)
			: m_KeyCode(keyCode) {}

		KeyCode m_KeyCode;
	};


	class KeyboardKeyPress : public KeyEvent
	{

	public:
		KeyboardKeyPress(KeyCode keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		virtual EventType GetEventType() const override { return EventType::KeyboardKeyPress; }
		int GetRepeatCount() const { return m_RepeatCount; }

	private:
		int m_RepeatCount;
	};


	class KeyboardKeyRelease : public KeyEvent
	{
	public:
		KeyboardKeyRelease(KeyCode keyCode)
			: KeyEvent(keyCode) {}

		virtual EventType GetEventType() const override { return EventType::KeyboardKeyRelease; }
	};

}