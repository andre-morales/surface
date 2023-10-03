#pragma once

namespace Glass {
	class Element;
	class LayoutType;

	class Layout {
	public:
		LayoutType& type;
		Element& element;

		Layout(LayoutType& type, Element& element);
		virtual ~Layout() = default;

		virtual void added(Element& child) = 0;
		virtual void relayout() = 0;

	private:
		Layout(const Layout&) = delete;
		Layout(Layout&&) = delete;
	};
}

