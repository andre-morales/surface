#pragma once
#include "Math/Vec3.h"

class Collision {
public:
	float penetration;
	Vector3f normal;

	// Correct position and velocity to "reverse" the collision.
	// The position vector will be adjust so the collision would not happen,
	// and the velocity will be adjusted to prevent further collision.
	void correctMotion(Vector3f& position, Vector3f& velocity);

	void correctGravityMotion(Vector3f& position, Vector3f& velocity);
};

