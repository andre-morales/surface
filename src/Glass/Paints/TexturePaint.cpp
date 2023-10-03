#include "TexturePaint.h"
#include "Glow/Glow.h"
#include "Glow/Texture.h"
#include "Glow/GLContext.h"
#include "GL/glew.h"
#include "Loggy.h"
#include <format>

static Loggy::Logger print{"GUI"};

namespace Glass {
	TexturePaint::TexturePaint(Glow::Texture& t) : 
		texture(t) {}

	void TexturePaint::paint(Math::Rectf area) {
		auto& ctx = *Glow::getContext();

		// Area
		auto [ax, ay, aw, ah] = area.arr;

		// Texture size
		float texWidth = texture.width;
		float texHeight = texture.height;
		
		// Position of each vertex: top, right, bottom, left
		float t = ay;
		float r = ax + aw;
		float b = ay + ah;
		float l = ax;

		// Corner sizes in pixels
		float lc = corners.left   * texWidth;
		float rc = corners.right  * texWidth;
		float tc = corners.top    * texHeight;
		float bc = corners.bottom * texHeight;

		const float verts[] = {
			// Left Top
			0,	0,
			lc,	0,
			lc,	tc,
			0,	tc,

			// Left Middle
			0,	tc,
			lc,	tc,
			lc,	ah - bc,
			0,	ah - bc,

			// Left Bottom
			0,	ah - bc,
			lc,	ah - bc,
			lc,	ah,
			0,	ah,

			// Center Top
			lc,		 0,
			aw - rc, 0,
			aw - rc, tc,
			lc,		 tc,

			// Center Middle
			lc,      tc,
			aw - rc, tc,
			aw - rc, ah - bc,
			lc,		 ah - bc,

			// Center Bottom
			lc,		 ah - bc,
			aw - rc, ah - bc,
			aw - rc, ah,
			lc,		 ah,

			// Right Top
			aw - rc, 0,
			aw,      0,
			aw,      tc,
			aw - rc, tc,

			// Right Middle
			aw - rc, tc,
			aw,      tc,
			aw,      ah - bc,
			aw - rc, ah - bc,

			// Right Bottom
			aw - rc, ah - bc,
			aw,      ah - bc,
			aw,      ah,
			aw - rc, ah,
		};

		const auto& sb = corners;

		// Half pixel fixes
		float xf = .5f / texWidth;
		float yf = .5f / texHeight;

		const float uvs[] = {
			// Left Top
			0,       0,
			sb.left, 0,
			sb.left, sb.top,
			0,       sb.top,

			// Left Middle
			0,       sb.top,
			sb.left, sb.top,
			sb.left, 1 - sb.bottom,
			0,       1 - sb.bottom,

			// Left Bottom
			0,       1 - sb.bottom,
			sb.left, 1 - sb.bottom,
			sb.left, 1,
			0,       1,

			// Center Top
			sb.left, 0,
			1 - sb.right, 0,
			1 - sb.right, sb.top,
			sb.left,	  sb.top,

			// Center Middle
			sb.left + xf,     sb.top + yf,
			1 - sb.left - xf, sb.top + yf,
			1 - sb.left - xf, 1 - sb.bottom - yf,
			sb.left + xf,     1 - sb.bottom - yf,

			// Center Bottom
			sb.left,	  1 - sb.bottom,
			1 - sb.right, 1 - sb.bottom,
			1 - sb.right, 1,
			sb.left,	  1,

			// Right Top
			1 - sb.right , 0,
			1 ,			   0,
			1 ,			   sb.top,
			1 - sb.right , sb.top,

			// Right Middle
			1 - sb.right, sb.top ,
			1 ,			   sb.top ,
			1 ,			   1 - sb.bottom,
			1 - sb.right, 1 - sb.bottom,

			// Right Bottom
			1 - sb.right , 1 - sb.bottom,
			1 ,			   1 - sb.bottom,
			1 ,			   1 ,
			1 - sb.right , 1 ,
		};

		ctx.enableVertsArray();
		ctx.enableUVsArray();

		texture.bind();
		
		glPushMatrix();
		glTranslatef(ax, ay, 0);
		glColor4f(1, 1, 1, 1);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glVertexPointer(2, GL_FLOAT, 0, verts);
		glDrawArrays(GL_QUADS, 0, 36);

		glPopMatrix();

		ctx.disableVertsArray();
		ctx.disableUVsArray();
	}
}