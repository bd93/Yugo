#pragma once

namespace Yugo
{

	typedef enum class MouseCode
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

}


#define MOUSE_BUTTON_0      ::Yugo::Mouse::Button0
#define MOUSE_BUTTON_1      ::Yugo::Mouse::Button1
#define MOUSE_BUTTON_2      ::Yugo::Mouse::Button2
#define MOUSE_BUTTON_3      ::Yugo::Mouse::Button3
#define MOUSE_BUTTON_4      ::Yugo::Mouse::Button4
#define MOUSE_BUTTON_5      ::Yugo::Mouse::Button5
#define MOUSE_BUTTON_6      ::Yugo::Mouse::Button6
#define MOUSE_BUTTON_7      ::Yugo::Mouse::Button7
#define MOUSE_BUTTON_LAST   ::Yugo::Mouse::ButtonLast
#define MOUSE_BUTTON_LEFT   ::Yugo::Mouse::ButtonLeft
#define MOUSE_BUTTON_RIGHT  ::Yugo::Mouse::ButtonRight
#define MOUSE_BUTTON_MIDDLE ::Yugo::Mouse::ButtonMiddle