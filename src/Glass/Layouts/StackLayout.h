#pragma once
#include "Layout.h"

namespace Glass {
	class StackLayout : public Layout {
	public:
		StackLayout(LayoutType& type, Element& element);

		void added(Element& element) override;
		void relayout() override;
	};
}

