#pragma once


namespace Yugo
{

	typedef enum class KeyCode
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

}

// From glfw3.h
#define KEY_SPACE           ::Yugo::Key::Space
#define KEY_APOSTROPHE      ::Yugo::Key::Apostrophe    /* ' */
#define KEY_COMMA           ::Yugo::Key::Comma         /* , */
#define KEY_MINUS           ::Yugo::Key::Minus         /* - */
#define KEY_PERIOD          ::Yugo::Key::Period        /* . */
#define KEY_SLASH           ::Yugo::Key::Slash         /* / */
#define KEY_0               ::Yugo::Key::D0
#define KEY_1               ::Yugo::Key::D1
#define KEY_2               ::Yugo::Key::D2
#define KEY_3               ::Yugo::Key::D3
#define KEY_4               ::Yugo::Key::D4
#define KEY_5               ::Yugo::Key::D5
#define KEY_6               ::Yugo::Key::D6
#define KEY_7               ::Yugo::Key::D7
#define KEY_8               ::Yugo::Key::D8
#define KEY_9               ::Yugo::Key::D9
#define KEY_SEMICOLON       ::Yugo::Key::Semicolon     /* ; */
#define KEY_EQUAL           ::Yugo::Key::Equal         /* = */
#define KEY_A               ::Yugo::Key::A
#define KEY_B               ::Yugo::Key::B
#define KEY_C               ::Yugo::Key::C
#define KEY_D               ::Yugo::Key::D
#define KEY_E               ::Yugo::Key::E
#define KEY_F               ::Yugo::Key::F
#define KEY_G               ::Yugo::Key::G
#define KEY_H               ::Yugo::Key::H
#define KEY_I               ::Yugo::Key::I
#define KEY_J               ::Yugo::Key::J
#define KEY_K               ::Yugo::Key::K
#define KEY_L               ::Yugo::Key::L
#define KEY_M               ::Yugo::Key::M
#define KEY_N               ::Yugo::Key::N
#define KEY_O               ::Yugo::Key::O
#define KEY_P               ::Yugo::Key::P
#define KEY_Q               ::Yugo::Key::Q
#define KEY_R               ::Yugo::Key::R
#define KEY_S               ::Yugo::Key::S
#define KEY_T               ::Yugo::Key::T
#define KEY_U               ::Yugo::Key::U
#define KEY_V               ::Yugo::Key::V
#define KEY_W               ::Yugo::Key::W
#define KEY_X               ::Yugo::Key::X
#define KEY_Y               ::Yugo::Key::Y
#define KEY_Z               ::Yugo::Key::Z
#define KEY_LEFT_BRACKET    ::Yugo::Key::LeftBracket   /* [ */
#define KEY_BACKSLASH       ::Yugo::Key::Backslash     /* \ */
#define KEY_RIGHT_BRACKET   ::Yugo::Key::RightBracket  /* ] */
#define KEY_GRAVE_ACCENT    ::Yugo::Key::GraveAccent   /* ` */
#define KEY_WORLD_1         ::Yugo::Key::World1        /* non-US #1 */
#define KEY_WORLD_2         ::Yugo::Key::World2        /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE          ::Yugo::Key::Escape
#define KEY_ENTER           ::Yugo::Key::Enter
#define KEY_TAB             ::Yugo::Key::Tab
#define KEY_BACKSPACE       ::Yugo::Key::Backspace
#define KEY_INSERT          ::Yugo::Key::Insert
#define KEY_DELETE          ::Yugo::Key::Delete
#define KEY_RIGHT           ::Yugo::Key::Right
#define KEY_LEFT            ::Yugo::Key::Left
#define KEY_DOWN            ::Yugo::Key::Down
#define KEY_UP              ::Yugo::Key::Up
#define KEY_PAGE_UP         ::Yugo::Key::PageUp
#define KEY_PAGE_DOWN       ::Yugo::Key::PageDown
#define KEY_HOME            ::Yugo::Key::Home
#define KEY_END             ::Yugo::Key::End
#define KEY_CAPS_LOCK       ::Yugo::Key::CapsLock
#define KEY_SCROLL_LOCK     ::Yugo::Key::ScrollLock
#define KEY_NUM_LOCK        ::Yugo::Key::NumLock
#define KEY_PRINT_SCREEN    ::Yugo::Key::PrintScreen
#define KEY_PAUSE           ::Yugo::Key::Pause
#define KEY_F1              ::Yugo::Key::F1
#define KEY_F2              ::Yugo::Key::F2
#define KEY_F3              ::Yugo::Key::F3
#define KEY_F4              ::Yugo::Key::F4
#define KEY_F5              ::Yugo::Key::F5
#define KEY_F6              ::Yugo::Key::F6
#define KEY_F7              ::Yugo::Key::F7
#define KEY_F8              ::Yugo::Key::F8
#define KEY_F9              ::Yugo::Key::F9
#define KEY_F10             ::Yugo::Key::F10
#define KEY_F11             ::Yugo::Key::F11
#define KEY_F12             ::Yugo::Key::F12
#define KEY_F13             ::Yugo::Key::F13
#define KEY_F14             ::Yugo::Key::F14
#define KEY_F15             ::Yugo::Key::F15
#define KEY_F16             ::Yugo::Key::F16
#define KEY_F17             ::Yugo::Key::F17
#define KEY_F18             ::Yugo::Key::F18
#define KEY_F19             ::Yugo::Key::F19
#define KEY_F20             ::Yugo::Key::F20
#define KEY_F21             ::Yugo::Key::F21
#define KEY_F22             ::Yugo::Key::F22
#define KEY_F23             ::Yugo::Key::F23
#define KEY_F24             ::Yugo::Key::F24
#define KEY_F25             ::Yugo::Key::F25

/* Keypad */
#define KEY_KP_0            ::Yugo::Key::KP0
#define KEY_KP_1            ::Yugo::Key::KP1
#define KEY_KP_2            ::Yugo::Key::KP2
#define KEY_KP_3            ::Yugo::Key::KP3
#define KEY_KP_4            ::Yugo::Key::KP4
#define KEY_KP_5            ::Yugo::Key::KP5
#define KEY_KP_6            ::Yugo::Key::KP6
#define KEY_KP_7            ::Yugo::Key::KP7
#define KEY_KP_8            ::Yugo::Key::KP8
#define KEY_KP_9            ::Yugo::Key::KP9
#define KEY_KP_DECIMAL      ::Yugo::Key::KPDecimal
#define KEY_KP_DIVIDE       ::Yugo::Key::KPDivide
#define KEY_KP_MULTIPLY     ::Yugo::Key::KPMultiply
#define KEY_KP_SUBTRACT     ::Yugo::Key::KPSubtract
#define KEY_KP_ADD          ::Yugo::Key::KPAdd
#define KEY_KP_ENTER        ::Yugo::Key::KPEnter
#define KEY_KP_EQUAL        ::Yugo::Key::KPEqual

#define KEY_LEFT_SHIFT      ::Yugo::Key::LeftShift
#define KEY_LEFT_CONTROL    ::Yugo::Key::LeftControl
#define KEY_LEFT_ALT        ::Yugo::Key::LeftAlt
#define KEY_LEFT_SUPER      ::Yugo::Key::LeftSuper
#define KEY_RIGHT_SHIFT     ::Yugo::Key::RightShift
#define KEY_RIGHT_CONTROL   ::Yugo::Key::RightControl
#define KEY_RIGHT_ALT       ::Yugo::Key::RightAlt
#define KEY_RIGHT_SUPER     ::Yugo::Key::RightSuper
#define KEY_MENU            ::Yugo::Key::Menu