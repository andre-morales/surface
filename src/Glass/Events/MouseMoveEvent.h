#pragma once
#include "Event.h"

namespace Glass {
	class MouseMoveEvent : Event {
	public:
		float x, y;

		MouseMoveEvent(float x, float y) : x(x), y(y){}
	};
}