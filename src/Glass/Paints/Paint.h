#pragma once
#include "Math/NamedTypes.h"

namespace Glass {
	class Paint {
	public:
		virtual void paint(Math::Rectf area) = 0;
	};
}
