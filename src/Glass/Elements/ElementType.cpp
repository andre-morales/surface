#include "ElementType.h"

namespace Glass {
	ElementType::ElementType(GUI& g, std::string_view name, std::type_index type) :
		gui(g),
		name(name),
		type(type)
	{}

	ElementType::ElementType(ElementType&& other) noexcept :
		gui(other.gui),
		name(other.name),
		type(other.type), 
		constructor(std::move(other.constructor))
	{}
}