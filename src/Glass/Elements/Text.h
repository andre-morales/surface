#pragma once
#include "Element.h"
#include "Glass/Constants.h"

namespace Glass {
	class Font;

	class Text : public Element {
	public:
		typedef Element parent_t;

		std::string text = "niceText";
		float scale = 1.0;
		TextVerticalAlign verticalAlignment = TextVerticalAlign::MIDDLE;
		TextHorizontalAlign horizontalAlignment = TextHorizontalAlign::CENTER;

		virtual void cloneInto(Element& clone) const override;
		void setSheetAttribs(const SheetElementAttribs&) override;
		void setFont(std::string_view fontName);
		std::string getFont() const;

		void paintSelf(Vector2f anchor) override;

	protected:
		Text(ElementType&);

	private:
		friend class GUI;

		std::string fontName;
		Font* font = nullptr;
	};
}

