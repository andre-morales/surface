#include "LayoutType.h"

namespace Glass {
	LayoutType::LayoutType(std::string_view name, std::type_index type) :
		name(name),
		type(type)
	{}

	LayoutType::LayoutType(LayoutType&& other) noexcept :
		name(other.name),
		type(other.type),
		constructor(std::move(other.constructor))
	{}
}