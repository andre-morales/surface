#pragma once
#include "Exception.h"

namespace Glass {
	class GlassException : public Exception {
	public:
		GlassException(const std::string&);
	};
}

