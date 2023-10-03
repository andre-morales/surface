#pragma once
#include "Paint.h"

namespace Glass {
	class ColorPaint : public Paint {
	public:
		Math::Colorf color;

		ColorPaint(const Math::Colorf& color);

		void paint(Math::Rectf area) override;
	};
}