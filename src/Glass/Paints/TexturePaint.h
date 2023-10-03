#pragma once
#include "Paint.h"

namespace Glow {
	class Texture;
}

namespace Glass {
	class TexturePaint : public Paint {
	public:
		Glow::Texture& texture;
		Math::Insetsf corners { 0, 0, 0, 0 };

		TexturePaint(Glow::Texture& texture);

		void paint(Math::Rectf area) override;
	};
}
