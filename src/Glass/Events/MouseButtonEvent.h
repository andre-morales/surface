#pragma once
#include "Event.h"
#include "Glow/Inputs.h"

namespace Glass {
	class MouseButtonEvent : public Event {
	public:
		Glow::InputAction action;
		Glow::MouseButton button;
		int mods;

		MouseButtonEvent(Glow::MouseButton btn, Glow::InputAction act, int mods) :
			button(btn),
			action(act),
			mods(mods) {}
	};
}

