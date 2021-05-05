#pragma once
#include <functional>
#include <map>


#include "Common.h"


struct GLFWwindow;


enum class EMouseButton
{
	LEFT = 0,  // GLFW_MOUSE_BUTTON_LEFT
	RIGHT = 1,  // GLFW_MOUSE_BUTTON_RIGHT
	MIDDLE = 2,  // GLFW_MOUSE_BUTTON_MIDDLE

	EXTRA1 = 3,  // GLFW_MOUSE_BUTTON_4
	EXTRA2 = 4,  // GLFW_MOUSE_BUTTON_5
	EXTRA3 = 5,  // GLFW_MOUSE_BUTTON_6
	EXTRA4 = 6,  // GLFW_MOUSE_BUTTON_7
	EXTRA5 = 7,  // GLFW_MOUSE_BUTTON_8
};

enum class EKeyButton
{
	BACKTICK = 96,  // GLFW_KEY_GRAVE_ACCENT
	
	NUM_0 = 48,  // GLFW_KEY_0
	NUM_1 = 49,  // GLFW_KEY_1
	NUM_2 = 50,  // GLFW_KEY_2
	NUM_3 = 51,  // GLFW_KEY_3
	NUM_4 = 52,  // GLFW_KEY_4
	NUM_5 = 53,  // GLFW_KEY_5
	NUM_6 = 54,  // GLFW_KEY_6
	NUM_7 = 55,  // GLFW_KEY_7
	NUM_8 = 56,  // GLFW_KEY_8
	NUM_9 = 57,  // GLFW_KEY_9
	
	MINUS = 45,  // GLFW_KEY_MINUS
	EQUAL = 61,  // GLFW_KEY_EQUAL

	A = 65,  // GLFW_KEY_A
	B = 66,  // GLFW_KEY_B
	C = 67,  // GLFW_KEY_C
	D = 68,  // GLFW_KEY_D
	E = 69,  // GLFW_KEY_E
	F = 70,  // GLFW_KEY_F
	G = 71,  // GLFW_KEY_G
	H = 72,  // GLFW_KEY_H
	I = 73,  // GLFW_KEY_I
	J = 74,  // GLFW_KEY_J
	K = 75,  // GLFW_KEY_K
	L = 76,  // GLFW_KEY_L
	M = 77,  // GLFW_KEY_M
	N = 78,  // GLFW_KEY_N
	O = 79,  // GLFW_KEY_O
	P = 80,  // GLFW_KEY_P
	Q = 81,  // GLFW_KEY_Q
	R = 82,  // GLFW_KEY_R
	S = 83,  // GLFW_KEY_S
	T = 84,  // GLFW_KEY_T
	U = 85,  // GLFW_KEY_U
	V = 86,  // GLFW_KEY_V
	W = 87,  // GLFW_KEY_W
	X = 88,  // GLFW_KEY_X
	Y = 89,  // GLFW_KEY_Y
	Z = 90,  // GLFW_KEY_Z

	SPACE = 32,  // GLFW_KEY_SPACE

	COMMA = 44,  // GLFW_KEY_COMMA
	PERIOD = 46,  // GLFW_KEY_PERIOD
	SLASH = 47,  // GLFW_KEY_SLASH
	SEMICOLON = 59,  // GLFW_KEY_SEMICOLON
	APOSTROPHE = 39,  // GLFW_KEY_APOSTROPHE
	LEFT_BRACKET = 91,  // GLFW_KEY_LEFT_BRACKET
	RIGHT_BRACKET = 93,  // GLFW_KEY_RIGHT_BRACKET
	BACKSLASH = 92,  // GLFW_KEY_BACKSLASH


	TAB = 258,  // GLFW_KEY_TAB
	CAPS_LOCK = 280,  // GLFW_KEY_CAPS_LOCK
	LEFT_SHIFT = 340,  // GLFW_KEY_LEFT_SHIFT
	LEFT_CONTROL = 341,  // GLFW_KEY_LEFT_CONTROL
	LEFT_ALT = 342,  // GLFW_KEY_LEFT_ALT
	LEFT_SUPER = 343,  // GLFW_KEY_LEFT_SUPER
	RIGHT_SHIFT = 344,  // GLFW_KEY_RIGHT_SHIFT
	RIGHT_CONTROL = 345,  // GLFW_KEY_RIGHT_CONTROL
	RIGHT_ALT = 346,  // GLFW_KEY_RIGHT_ALT
	RIGHT_SUPER = 347,  // GLFW_KEY_RIGHT_SUPER
	MENU = 348,  // GLFW_KEY_MENU

	ENTER = 257,  // GLFW_KEY_ENTER
	BACKSPACE = 259,  // GLFW_KEY_BACKSPACE

	ESCAPE = 256,  // GLFW_KEY_ESCAPE
	
	F1 = 290,  // GLFW_KEY_F1
	F2 = 291,  // GLFW_KEY_F2
	F3 = 292,  // GLFW_KEY_F3
	F4 = 293,  // GLFW_KEY_F4
	F5 = 294,  // GLFW_KEY_F5
	F6 = 295,  // GLFW_KEY_F6
	F7 = 296,  // GLFW_KEY_F7
	F8 = 297,  // GLFW_KEY_F8
	F9 = 298,  // GLFW_KEY_F9
	F10 = 299,  // GLFW_KEY_F10
	F11 = 300,  // GLFW_KEY_F11
	F12 = 301,  // GLFW_KEY_F12
	F13 = 302,  // GLFW_KEY_F13
	F14 = 303,  // GLFW_KEY_F14
	F15 = 304,  // GLFW_KEY_F15
	F16 = 305,  // GLFW_KEY_F16
	F17 = 306,  // GLFW_KEY_F17
	F18 = 307,  // GLFW_KEY_F18
	F19 = 308,  // GLFW_KEY_F19
	F20 = 309,  // GLFW_KEY_F20
	F21 = 310,  // GLFW_KEY_F21
	F22 = 311,  // GLFW_KEY_F22
	F23 = 312,  // GLFW_KEY_F23
	F24 = 313,  // GLFW_KEY_F24
	F25 = 314,  // GLFW_KEY_F25

	PRINT_SCREEN = 283,  // GLFW_KEY_PRINT_SCREEN
	SCROLL_LOCK = 281,  // GLFW_KEY_SCROLL_LOCK
	PAUSE = 284,  // GLFW_KEY_PAUSE

	INSERT = 260,  // GLFW_KEY_INSERT
	DELETE = 261,  // GLFW_KEY_DELETE
	HOME = 268,  // GLFW_KEY_HOME
	END = 269,  // GLFW_KEY_END
	PAGE_UP = 266,  // GLFW_KEY_PAGE_UP
	PAGE_DOWN = 267,  // GLFW_KEY_PAGE_DOWN

	UP = 265,  // GLFW_KEY_UP
	DOWN = 264,  // GLFW_KEY_DOWN
	LEFT = 263,  // GLFW_KEY_LEFT
	RIGHT = 262,  // GLFW_KEY_RIGHT

	NUM_LOCK = 282,  // GLFW_NUM_LOCK

	NUMPAD_0 = 320,  // GLFW_KEY_KP_0
	NUMPAD_1 = 321,  // GLFW_KEY_KP_1
	NUMPAD_2 = 322,  // GLFW_KEY_KP_2
	NUMPAD_3 = 323,  // GLFW_KEY_KP_3
	NUMPAD_4 = 324,  // GLFW_KEY_KP_4
	NUMPAD_5 = 325,  // GLFW_KEY_KP_5
	NUMPAD_6 = 326,  // GLFW_KEY_KP_6
	NUMPAD_7 = 327,  // GLFW_KEY_KP_7
	NUMPAD_8 = 328,  // GLFW_KEY_KP_8
	NUMPAD_9 = 329,  // GLFW_KEY_KP_9

	NUMPAD_DECIMAL = 330,  // GLFW_KEY_KP_DECIMAL
	NUMPAD_DIVIDE = 331,  // GLFW_KEY_KP_DIVIDE
	NUMPAD_MULTIPLY = 332,  // GLFW_KEY_KP_MULTIPLY
	NUMPAD_SUBTRACT = 333,  // GLFW_KEY_KP_SUBTRACT
	NUMPAD_ADD = 334,  // GLFW_KEY_KP_ADD
	NUMPAD_ENTER = 335,  // GLFW_KEY_KP_ENTER
	NUMPAD_EQUAL = 336,  // GLFW_KEY_KP_EQUAL
	
	
	/// REDIRECTS
	BACKQUOTE = BACKTICK,
	GRAVE_ACCENT = BACKTICK,
	DASH = MINUS,
	DOT = PERIOD,
	QUOTE = APOSTROPHE,
	ESC = ESCAPE,
	PRINT_SCREEN_SHOT = PRINT_SCREEN,
	SCREENSHOT = PRINT_SCREEN,
	BREAK = PAUSE,
	PAUSE_BREAK = PAUSE,
	UP_ARROW = UP,
	ARROW_UP = UP,
	DOWN_ARROW = DOWN,
	ARROW_DOWN = DOWN,
	LEFT_ARROW = LEFT,
	ARROW_LEFT = LEFT,
	RIGHT_ARROW = RIGHT,
	ARROW_RIGHT = RIGHT,
	SHIFT = LEFT_SHIFT,
	LSHIFT = LEFT_SHIFT,
	CONTROL = LEFT_CONTROL,
	LCONTROL = LEFT_CONTROL,
	ALT = LEFT_ALT,
	LALT = LEFT_ALT,
	SUPER = LEFT_SUPER,
	LSUPER = LEFT_SUPER,
	WINDOWS = LEFT_SUPER,
	COMMAND = LEFT_SUPER,
	RSHIFT = RIGHT_SHIFT,
	RCONTROL = RIGHT_CONTROL,
	RALT = RIGHT_ALT,
	RSUPER = RIGHT_ALT,
	NUMPAD_DOT = NUMPAD_DECIMAL,
	NUMPAD_PERIOD = NUMPAD_DECIMAL,
	NUMPAD_SLASH = NUMPAD_DIVIDE,
	NUMPAD_ASTERISK = NUMPAD_MULTIPLY,
	NUMPAD_MINUS = NUMPAD_SUBTRACT,
	NUMPAD_DASH = NUMPAD_SUBTRACT,
	NUMPAD_PLUS = NUMPAD_ADD,
};

enum class EKeyModifier
{
	NONE = 0x0000,
	
	SHIFT = 0x0001,  // GLFW_MOD_SHIFT
	CONTROL = 0x0002,  // GLFW_MOD_CONTROL
	ALT = 0x0004,  // GLFW_MOD_ALT
	
	SUPER = 0x0008,  // GLFW_MOD_SUPER
	
	CAPS_LOCK = 0x0010,  // GLFW_MOD_CAPS_LOCK
	NUM_LOCK = 0x0020,  // GLFW_MOD_NUM_LOCK


	/// REDIRECTS
	WINDOWS = SUPER,
};

enum class EButtonAction
{
	PRESS = 1,  // GLFW_PRESS
	RELEASE = 0,  // GLFW_RELEASE
	REPEAT = -1
};


class InputManager
{
public:
	using MouseMoveCallback = std::function<void(float moveX, float moveY)>;
	using MouseButtonCallback = std::function<void(EMouseButton button, EButtonAction action, EKeyModifier mod)>;
	using MouseScrollCallback = std::function<void(float xAmount, float yAmount)>;
	using KeyCallback = std::function<void(EKeyButton key, EButtonAction action, EKeyModifier mod)>;
	
	[[nodiscard]] static InputCallbackHandle AddMouseMoveCallback(MouseMoveCallback&& callback);
	[[nodiscard]] static InputCallbackHandle AddMouseButtonCallback(MouseButtonCallback&& callback);
	[[nodiscard]] static InputCallbackHandle AddMouseScrollCallback(MouseScrollCallback&& callback);
	[[nodiscard]] static InputCallbackHandle AddKeyCallback(KeyCallback&& callback);

	static void RemoveMouseMoveCallback(InputCallbackHandle callbackHandle) noexcept;
	static void RemoveMouseButtonCallback(InputCallbackHandle callbackHandle) noexcept;
	static void RemoveMouseScrollCallback(InputCallbackHandle callbackHandle) noexcept;
	static void RemoveKeyCallback(InputCallbackHandle callbackHandle) noexcept;

	[[nodiscard]] static bool IsKeyDown(EKeyButton button) noexcept;
	[[nodiscard]] static bool IsMouseDown(EMouseButton button) noexcept;

	[[nodiscard]] static std::pair<float, float> GetCursorPosition() noexcept;


	/// Do not call the following functions directly
	static void _Update();
	
	static void _CursorPosCallback(GLFWwindow*, double x, double y);
	static void _MouseButtonCallback(GLFWwindow*, int button, int action, int mod);
	static void _MouseScrollCallback(GLFWwindow*, double xAmount, double yAmount);
	static void _KeyCallback(GLFWwindow*, int key, int, int action, int mod);



private:
	inline static std::map<InputCallbackHandle, MouseMoveCallback> mouseMoveCallbacks;
	inline static std::map<InputCallbackHandle, MouseButtonCallback> mouseButtonCallbacks;
	inline static std::map<InputCallbackHandle, MouseScrollCallback> mouseScrollCallbacks;
	inline static std::map<InputCallbackHandle, KeyCallback> keyCallbacks;

	inline static std::map<EMouseButton, bool> mouseButtonDownMap;
	inline static std::map<EKeyButton, bool> keyButtonDownMap;

	inline static std::pair<float, float> mousePosition;

	inline static InputCallbackHandle nextCallbackHandle = 0;
};



constexpr EKeyModifier operator|(const EKeyModifier a, const EKeyModifier b) noexcept
{
	using T = std::underlying_type_t<EKeyModifier>;
	return static_cast<EKeyModifier>(static_cast<T>(a) | static_cast<T>(b));
}
