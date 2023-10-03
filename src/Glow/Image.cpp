#include "Image.h"
#include "Exception.h"
extern "C" {
	#include "Lodepng/lodepng.h"
}

using namespace std::string_literals;

namespace Glow {
	Image::Image() {}

	Image::Image(const char* name) {
		loadFromFile(name);
	}

	void Image::loadFromFile(const char* path) {
		uint8_t* img = nullptr;
		
		uint32_t err = lodepng_decode32_file(&img, &width, &height, path);

		if (err) {
			throw Exception("PNG Loading Error: "s + lodepng_error_text(err));
		}

		size_t len = 4LL * width * height;

		data.reset(new uint8_t[len]);
		memcpy(data.get(), img, len);
	}
}