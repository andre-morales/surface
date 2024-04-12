#pragma once
#include <cassert>

template <class T, size_t Size>
class StaticVector {
public:
	T elements[Size];
	size_t length = 0; 

	constexpr void push(T element) {
		assert(length < Size);

		elements[length] = element;
		length++;
	}

	constexpr void clear() {
		length = 0;
	}
};

