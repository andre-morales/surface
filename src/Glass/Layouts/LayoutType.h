#pragma once
#include <string>
#include <typeindex>
#include <functional>

namespace Glass {
	class Element;
	class Layout;

	class LayoutType {
	public:
		typedef std::function<Layout* (LayoutType&, Element&)> ConstructorFn;

		const std::string name;
		const std::type_index type;
		ConstructorFn constructor;

		LayoutType(std::string_view name, std::type_index typeIndex);
		LayoutType(LayoutType&&) noexcept;

	private:
		LayoutType(const LayoutType&) = delete;
		
	};
}
