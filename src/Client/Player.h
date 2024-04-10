#pragma once
#include "Math/Vec3.h"
#include "Math/Vec2.h"
#include "Pointers.h"
#include <vector>
#include <array>

class Capsule;
class AABB;
class Session;

class Player {
public:
	Session& session;
	Vector3f position;
	Vector2f rotation;
	Vector3f velocity;

	Player(Session&);
	~Player();

	void doPhysics(float timeDelta);

	std::array<Vector3f, 24> getBoxColliderLines();
	std::vector<std::array<Vector3f, 3>> collidingTriangles;

private:
	unique<AABB> boxCollider;
	unique<Capsule> capsuleCollider;
};

