#pragma once
#include "types.h"

class Noise {
public:
	uint16 samples[4096];

	Noise(uint64);
	~Noise();
	double get(double, double, double);
};
