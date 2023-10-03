#include "ColorPaint.h"
#include "GL/glew.h"
#include "Glow\Glow.h"
#include "Glow\GLContext.h"

namespace Glass {
	ColorPaint::ColorPaint(const Math::Colorf& c) :
	color(c) {}

	void ColorPaint::paint(Math::Rectf area) {
		auto& ctx = *Glow::getContext();
		
		const auto& [x, y, w, h] = area.arr;
		
		glDisable(GL_TEXTURE_2D);
		
		glBegin(GL_QUADS);
		glColor4f(color.r, color.g, color.b, color.a);		
		glVertex2f(x,     y);
		glVertex2f(x,     y + h);
		glVertex2f(x + w, y + h);
		glVertex2f(x + w, y);
		glEnd();

		glEnable(GL_TEXTURE_2D);
	}
}