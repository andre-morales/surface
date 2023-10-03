#include "StackLayout.h"
#include "Glass/Elements/Element.h"

namespace Glass {
	StackLayout::StackLayout(LayoutType& type, Element& element) :
		Layout(type, element) {}

	void StackLayout::added(Element& element) {
		auto bounds = element.getBounds();
		element.setBounds(bounds);
	}
		
	void StackLayout::relayout() {
		auto bounds = element.getBounds();

		for (const auto& child : element.getChildren()) {
			child->setSize(bounds.w, bounds.h);
		}
	}
}