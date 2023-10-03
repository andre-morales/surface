#pragma once

namespace Glow {
	enum class InputAction {
		RELEASE = 0, PRESS = 1, REPEAT = 2
	};

	enum class InputKey {
		SPACE = 32,
		A = 65, B = 66, C = 67, D = 68,
		S = 83, W = 87,

		ESC = 256,

		F1 = 290,

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