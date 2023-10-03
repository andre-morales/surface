#pragma once
#include <cstdint>
#include "Pointers.h"

/**
* 
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
**/

namespace Glow {
	class Image;

	class Texture {
	public:
		enum class Filtering {
			NEAR = 0x2600,
			LINEAR = 0x2601,

			MMAP_NEAREST_NEAREST = 0x2700,
			MMAP_LINEAR_NEAREST = 0x2701,
			MMAP_NEAREST_LINEAR = 0x2702,
			MMAP_LINEAR_LINEAR = 0x2703, 
		};

		enum class Wrapping {
			CLAMP = 0x812F,
			REPEAT = 0x2901
		};

		uint32_t glId = 0;
		uint32_t width = 0, height = 0;

		Texture();
		Texture(Texture&&) noexcept;
		Texture(Image&& image);
		Texture(const char* filePath);

		void setImage(Image&& img);
		void setFiltering(Filtering min, Filtering mag);
		void setWrapping(Wrapping s, Wrapping t);
		void bind();

	private:
		bool imageChanged = false;
		bool configChanged = true;
		unique<uint8_t[]> image;
		Filtering minFilter = Filtering::LINEAR;
		Filtering magFilter = Filtering::LINEAR;
		Wrapping sWrapping = Wrapping::CLAMP;
		Wrapping tWrapping = Wrapping::CLAMP;

		Texture(const Texture&) = delete;
	};
}
