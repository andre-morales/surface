#include "Text.h"
#include "Glass/Font.h"
#include "Glass/GUI.h"
#include "Glass/SheetElementAttribs.h"

namespace Glass {
	Text::Text(ElementType& et) : Element(et) {}

	void Text::setSheetAttribs(const SheetElementAttribs& attr) {
		Element::setSheetAttribs(attr);

		auto fontAttr = attr.get("font");
		if (fontAttr) {
			fontName = *fontAttr;
		}

		auto valueAttr = attr.get("value");
		if (valueAttr) {
			text = *valueAttr;
		}

		auto textSizeAttr = attr.getFloat("font-size");
		if (textSizeAttr) {
			scale = *textSizeAttr;
		}

		auto halignAttr = attr.get("halign");
		if (halignAttr) {
			if (*halignAttr == "left") {
				horizontalAlignment = TextHorizontalAlign::LEFT;
			}
			if (*halignAttr == "center") {
				horizontalAlignment = TextHorizontalAlign::CENTER;
			}
			if (*halignAttr == "right") {
				horizontalAlignment = TextHorizontalAlign::RIGHT;
			}
		}

		auto valignAttr = attr.get("valign");
		if (valignAttr) {
			if (*valignAttr == "top") {
				verticalAlignment = TextVerticalAlign::TOP;
			}
			if (*valignAttr == "middle") {
				verticalAlignment = TextVerticalAlign::MIDDLE;
			}
			if (*valignAttr == "bottom") {
				verticalAlignment = TextVerticalAlign::BOTTOM;
			}
		}
	}

	void Text::cloneInto(Element& clone) const {
		Text& other = castRef<Text>(clone);
		Element::cloneInto(other);

		other.text = text;
		other.font = font;
		other.fontName = fontName;
		other.scale = scale;
		other.verticalAlignment = verticalAlignment;
		other.horizontalAlignment = horizontalAlignment;
	}

	void Text::paintSelf(Vector2f anchor) {
		Element::paintSelf(anchor);
		

		// If no font defined, don't draw anything else
		if (fontName.empty()) return;

		if (!font) {
			font = gui.getFont(fontName);

			if (!font) throw GlassException("No font named [" + fontName + "]");
		}
		
		auto metrics = font->measure(text);
		float x = 0, y = 0;
		switch (horizontalAlignment) {
		case TextHorizontalAlign::CENTER:
			x = (bounds.w - metrics.largestLineSize * scale * font->charSize.w) / 2.0;
			break;
		case TextHorizontalAlign::RIGHT:
			x = bounds.w - metrics.largestLineSize * scale * font->charSize.w;
			break;
		}
		
		switch (verticalAlignment) {
		case TextVerticalAlign::MIDDLE:
			y = (bounds.h - metrics.totalLines * scale * font->charSize.h) / 2.0;
			break;
		case TextVerticalAlign::BOTTOM:
			y = bounds.h - metrics.totalLines * scale * font->charSize.h;
			break;
		}

		anchor.x += x;
		anchor.y += y;
		font->drawText(text, anchor, scale, horizontalAlignment);
	}

	void Text::setFont(std::string_view name) {
		font = nullptr;
		fontName = name;
	}

	std::string Text::getFont() const {
		return fontName;
	}
}