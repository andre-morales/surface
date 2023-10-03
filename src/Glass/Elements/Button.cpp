#include "Button.h"
#include "Text.h"
#include "ElementType.h"
#include "Glass/GUI.h"
#include "Glass/Elements/Text.h"
#include "Glass/SheetElementAttribs.h"
#include "Glass/Layouts/StackLayout.h"

namespace Glass {
	Button::Button(ElementType& t) : Element(t) {
		auto textElem = gui.make<Text>();
		textElem->text = "Button";
		textElem->scale = 1.2;
		textElement = textElem.get();
		
		setLayout<StackLayout>();
		add(std::move(textElem));
	}

	void Button::setSheetAttribs(const SheetElementAttribs& attr) {
		Element::setSheetAttribs(attr);

		auto fontAttr = attr.get("font");
		if (fontAttr) {
			textElement->setFont(*fontAttr);
		}

		auto textAttr = attr.get("text");
		if (textAttr) {
			textElement->text = *textAttr;
		}

		auto textSizeAttr = attr.getFloat("font-size");
		if (textSizeAttr) {
			textElement->scale = *textSizeAttr;
		}
	}

	void Button::cloneInto(Element& clone) const {
		Button& other = castRef<Button>(clone);
		Element::cloneInto(other);

		other.setText(textElement->text);
		other.setFont(getFont());
		other.textElement->scale = textElement->scale;
		
	}

	void Button::setText(std::string_view txt) {
		auto textElem = (Text*)children.at(0).get();
		textElem->text = txt;
	}

	void Button::setFont(std::string_view fontName) {
		textElement->setFont(fontName);
	}

	std::string Button::getFont() const {
		return textElement->getFont();
	}
}