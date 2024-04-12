#include "Body.h"

namespace Physics {
	Body::Body(float m, bool c, int t)
		: mass(m), isConvex(c), tag(t) {
		
	}
}