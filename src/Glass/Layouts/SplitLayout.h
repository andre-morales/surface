#pragma once
#include "Glass/Constants.h"
#include "Layout.h"
#include <vector>

namespace Glass {
	class SplitLayout : public Layout {
	public:
		//Axis axis = Axis::HORIZONTAL;
		//bool fillOpposite = false;
		std::vector<float> splits;

		SplitLayout(LayoutType& type, Element& element);

		void added(Element& element) override;
		void relayout() override;

	private:

	};
}

