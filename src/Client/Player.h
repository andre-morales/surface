#pragma once
#include "Math/Vec3.h"
#include "Math/Vec2.h"
#include "Pointers.h"
#include <vector>
#include <array>

class AABB;
class Session;

class Player {
public:
	Session& session;
	Vector3f position;
	Vector2f rotation;
	Vector3f velocity;

	Player(Session&);

	void doPhysics(float timeDelta);
	std::array<Vector3f, 24> getColliderBoxLines();

	std::vector<std::array<Vector3f, 3>> collidingTriangles;

private:
	unique<AABB> collider;
};

