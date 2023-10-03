#pragma once
#include "Event.h"

namespace Glass {
	class MouseOverEvent : public Event {
	public:
		bool leaving;

		MouseOverEvent(bool leaving) : leaving(leaving) {}

		bool isEntering() { return !leaving; }
		bool isLeaving() { return leaving; }
	};
}

