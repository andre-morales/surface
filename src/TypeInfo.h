#pragma once
#include <string>
#include <typeindex>
#include <functional>

namespace Glass {
	class GUI;
	class Element;

	class ElementType {
	public:
		typedef std::function<Element* (ElementType&)> ConstructorFn;
		typedef std::function<void(Element&)> StylerFn;

		GUI& gui;
		const std::string name;
		const std::type_index type;
		ConstructorFn constructor;

		ElementType(GUI& gui, std::string_view name, std::type_index typeIndex);
		ElementType(ElementType&&) noexcept;

	private:
		ElementType(const ElementType&) = delete;

	};
}
