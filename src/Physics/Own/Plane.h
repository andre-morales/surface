#pragma once
#include "Math/vec3.h"

class Plane {
public:
	Vector3f direction;
	float distance = 0;

	void fromTriangle(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
		direction = Vector3f::cross(b - a, c - a).normalize();
		distance = Vector3f::dot(direction, a);
	}
};

