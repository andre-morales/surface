#pragma once
#include "Types.h"

class Block {
public:
	uint8_t id;

	Block(uint8_t id_) : id(id_) {}

	operator uint8_t() const {
		return id;
	}
};