#pragma once
#include <vector>

namespace Glass {
	class TextMetrics {
	public:
		std::vector<int> lineSizes;
		int largestLineSize;
		int totalLines;
	};
}