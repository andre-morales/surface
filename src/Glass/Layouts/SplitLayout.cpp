#include "SplitLayout.h"
#include "Glass/Elements/Element.h"
#include "Glass/SheetElementAttribs.h"

namespace Glass {
	SplitLayout::SplitLayout(LayoutType& type, Element& element) :
		Layout(type, element) {

		const auto& attrs = element.getSheetAttribs();
		auto splitsAttr = attrs->get("splits");
		if (splitsAttr) {
			auto splitsVec = attrs->asFloatVector(*splitsAttr);
			if (splitsVec) {
				splits = *splitsVec;
			}
		}
	}

	void SplitLayout::added(Element& element) {
		//auto bounds = element.getBounds();
		//element.setBounds(bounds);
	}

	void SplitLayout::relayout() {
		auto bounds = element.getBounds();

		float accumSplitValue = 0;
		
		int i = 0;
		for (const auto& child : element.getChildren()) {
			float splitValue = splits.at(i++);

			float childWidth = bounds.w * (splitValue - accumSplitValue);

			auto chBounds = child->getBounds();
			chBounds.w = childWidth;
			chBounds.x = accumSplitValue * bounds.w;
			child->setBounds(chBounds);

			accumSplitValue += splitValue;
		}
	}
}