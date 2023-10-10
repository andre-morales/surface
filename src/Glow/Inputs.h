#pragma once

namespace Glow {
	enum class InputAction {
		RELEASE = 0, PRESS = 1, REPEAT = 2
	};

	enum class InputKey : int {
		SPACE = 32,

		NUM_0 = 48, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5,
		NUM_6, NUM_7, NUM_8, NUM_9,

		A = 65, B = 66, C = 67, D = 68,
		S = 83, W = 87,

		ESC = 256,

		F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

		L_SHIFT = 340,
		L_CTRL = 341,
		L_ALT = 342
	};

	enum class MouseButton {
		LEFT = 0,
		RIGHT,
		MIDDLE,
		BTN_4, BTN_5, BTN_6,
		BTN_7, BTN_8
	};
}