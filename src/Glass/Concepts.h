#pragma once
#include <type_traits>

namespace Glass {
	class Element;
	class Layout;

	template <class T>
	concept ELEMENT = std::is_base_of<Element, T>::value;

	template <class T>
	concept LAYOUT = std::is_base_of<Layout, T>::value;
}