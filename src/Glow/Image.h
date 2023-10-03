#pragma once
#include <string>
#include "Pointers.h"

namespace Glow {
	class Image {
	public:
		Image();
		Image(const char* filePath);

		void loadFromFile(const char* path);
		uint32_t width = 0, height = 0;
		unique<uint8_t[]> data = nullptr;
		
	private:
		Image(const Image&) = delete;
		Image(Image&&) = delete;
	};
}

