#pragma once
#include "Element.h"

namespace Glass {
	class Text;

	class Button : public Element {
	public:
		typedef Element parent_t;

		virtual void cloneInto(Element& clone) const override;
		void setSheetAttribs(const SheetElementAttribs&) override;
		void setText(std::string_view text);

		void setFont(std::string_view fontName);
		std::string getFont() const;
	protected:
		Button(ElementType&);

	private:
		friend class GUI;

		Text* textElement;
	};
}

