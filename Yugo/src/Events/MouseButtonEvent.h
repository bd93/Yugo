#pragma once
#include "Event.h"


namespace Yugo
{


	class ButtonEvent : public Event
	{
	public:
		MouseCode GetButtonCode() const { return m_ButtonCode; }

	protected:
		ButtonEvent(MouseCode buttonCode)
			: m_ButtonCode(buttonCode){}

		MouseCode m_ButtonCode;
	};


	class MouseButtonPress : public ButtonEvent
	{

	public:
		MouseButtonPress(MouseCode buttonCode, int repeatCount)
			: ButtonEvent(buttonCode), m_RepeatCount(repeatCount) {}

		virtual EventType GetEventType() const override { return EventType::MouseButtonPress; }
		int GetRepeatCount() const { return m_RepeatCount; }

	private:
		int m_RepeatCount;
	};


	class MouseButtonRelease : public ButtonEvent
	{
	public:
		MouseButtonRelease(MouseCode buttonCode)
			: ButtonEvent(buttonCode) {}

		virtual EventType GetEventType() const override { return EventType::MouseButtonRelease; }
	};


	class MouseScroll : public Event
	{
	public:
		MouseScroll(float offsetX, float offsetY)
			: m_OffsetX(offsetX), m_OffsetY(offsetY) {}

		virtual EventType GetEventType() const override { return EventType::MouseScroll; }
		float GetOffsetX() const { return m_OffsetX; }
		float GetOffsetY() const { return m_OffsetY; }

	private:
		float m_OffsetX;
		float m_OffsetY;
	};


	class MouseMove : public Event
	{
	public:
		MouseMove(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		virtual EventType GetEventType() const override { return EventType::MouseMove; }
		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

	private:
		float m_MouseX, m_MouseY;
	};

}