#include "Texture.h"
#include "Image.h"
#include "GL/glew.h"
#include "Loggy.h"

static Loggy::Logger print {"Texture"};

namespace Glow {
	Texture::Texture() {}

	Texture::Texture(Texture&& other) noexcept :
		glId(other.glId),
		width(other.width),
		height(other.height), 
		imageChanged(other.imageChanged), 
		configChanged(other.configChanged), 
		image(std::move(other.image)),
		minFilter(other.minFilter), 
		magFilter(other.magFilter),
		sWrapping(other.sWrapping),
		tWrapping(other.tWrapping) {
		other.glId = 0;
	}

	Texture::Texture(Image&& img) {
		setImage(std::move(img));
	}

	Texture::Texture(const char* file) {
		setImage(Image(file));
	}

	void Texture::setImage(Image&& img) {
		imageChanged = true;
		this->image = std::move(img.data);
		width = img.width;
		height = img.height;
		img.width = img.height = 0;
	}

	void Texture::setFiltering(Filtering min, Filtering mag) {
		minFilter = min;
		magFilter = mag;
		configChanged = true;
	}

	void Texture::setWrapping(Wrapping s, Wrapping t) {
		sWrapping = s;
		tWrapping = t;
		configChanged = true;
	}

	void Texture::bind() {
		if (glId == 0) {
			glGenTextures(1, &glId);
			glBindTexture(GL_TEXTURE_2D, glId);

			print("Generated texture: ", glId);
		} else {
			glBindTexture(GL_TEXTURE_2D, glId);
		}

		if (imageChanged) {
			imageChanged = false;

			if (image) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.get());
			}
		}

		if (configChanged) {
			configChanged = false;
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (uint32_t) sWrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (uint32_t) tWrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (uint32_t) minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (uint32_t) magFilter);
		}
	}
}