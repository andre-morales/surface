#include "Font.h"
#include "Glow/Texture.h"
#include "Glow/Glow.h"
#include "Glow/GLContext.h"
#include "Glass/GlassException.h"
#include "Strings.h"
#include "GL/glew.h"
#include <cassert>

using namespace std::string_literals;

namespace Glass {
	Font::Font(std::string_view n, std::string_view texName, Math::Sizei cSize) :
		name(n), textureName(texName), charSize(cSize) {}

	Font::Font(Font&& o) noexcept :
		name(o.name), textureName(o.textureName),
		charSize(o.charSize), bitmap(o.bitmap) {
	}

	Math::Pointf Font::getCharUV(char c) {
		assert(bitmap != nullptr);

		int ci = c - 32;

		int charLineSize = bitmap->width / charSize.w;
		
		int cx = ci % charLineSize;
		int cy = ci / charLineSize;
		
		float u = charSize.w / (float)bitmap->width * cx;
		float v = charSize.h / (float)bitmap->height * cy;
		return { u + 0.5f / bitmap->width, v + 0.5f / bitmap->height };
	}

	void Font::drawText(std::string_view text, Math::Pointf position, float scale, TextHorizontalAlign alignment) {
		bindTexture();

		auto metrics = measure(text);

		glPushMatrix();
		glTranslatef(position.x, position.y, 0);
		glScalef(charSize.w * scale, charSize.h * scale, 1);

		float cw = charSize.w / (float)bitmap->width;
		float ch = charSize.h / (float)bitmap->height;

		int lineN = 0;
		float lineX = 0;
		for (int i = 0; i < text.size(); i++) {
			char c = text[i];

			if (c == '\n') lineN++;
			if (i == 0 || c == '\n') {
				switch (alignment) {
				case TextHorizontalAlign::LEFT:
					lineX = 0;
					break;
				case TextHorizontalAlign::CENTER:
					lineX = (metrics.largestLineSize - metrics.lineSizes[lineN]) / 2.0f;
					break;
				case TextHorizontalAlign::RIGHT:
					lineX = metrics.largestLineSize - metrics.lineSizes[lineN];
					break;
				}
			}

			if (c == '\n') continue;

			auto uv = getCharUV(c);
			auto [u, v] = uv.vec;

			glBegin(GL_QUADS);

			glColor4f(1, 1, 1, 1);
			glTexCoord2f(u, v);
			glVertex2f(lineX + 0, lineN + 0);

			glColor4f(1, 1, 1, 1);
			glTexCoord2f(u + cw, v);
			glVertex2f(lineX + 1, lineN + 0);

			glColor4f(1, 1, 1, 1);
			glTexCoord2f(u + cw, v + ch);
			glVertex2f(lineX + 1, lineN + 1);

			glColor4f(1, 1, 1, 1);
			glTexCoord2f(u, v + ch);
			glVertex2f(lineX + 0, lineN + 1);
			glEnd();

			lineX++;
		}

		glPopMatrix();

	}

	void Font::bindTexture() {
		// Bind texture
		if (!bitmap) {
			auto* ctx = Glow::getContext();
			assert(ctx != nullptr);

			auto* tex = ctx->getTexture(textureName);
			if (!tex) {
				throw GlassException(str::concat("Font drawing failed. Texture named [", textureName, "] not registered in the current context."));
			}

			bitmap = tex;
		}
		bitmap->bind();
	}

	TextMetrics Font::measure(std::string_view text) {
		std::vector<int> lineSizes;
		int lineSize = 0;
		int largestLine = 0;

		int lines = 1;

		for (char c : text) {
			if (c == '\n') {
				if (lineSize > largestLine) {
					largestLine = lineSize;
				}

				lineSizes.push_back(lineSize);
				lineSize = 0;
				lines++;
			} else {
				lineSize++;
			}
		}

		lineSizes.push_back(lineSize);
		if (lineSize > largestLine) {
			largestLine = lineSize;
		}

		return TextMetrics {
			.lineSizes = lineSizes,
			.largestLineSize = largestLine,
			.totalLines = lines
		};
	}
}
