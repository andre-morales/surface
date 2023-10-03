#pragma once
#include "Math/NamedTypes.h"
#include "TextMetrics.h"
#include "Glass/Constants.h"
#include <string>

namespace Glow {
	class Texture;
}

namespace Glass {
	class Font {
	public:
		const std::string name;
		const std::string textureName;
		const Math::Sizei charSize;

		Font(std::string_view name, std::string_view textureName, Math::Sizei charSize);
		Font(Font&&) noexcept;
		
		void drawText(std::string_view text, Math::Pointf position, float scale, TextHorizontalAlign alignment = TextHorizontalAlign::CENTER);
		static TextMetrics measure(std::string_view);

	private:
		Glow::Texture* bitmap = nullptr;

		Font(const Font&) = delete;
		Math::Pointf getCharUV(char);
		void bindTexture();
	};
}

