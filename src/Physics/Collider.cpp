#include "Collider.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace Physics {
	Collider::Collider() {}
	Collider::Collider(unique<btCollisionShape>&& s)
		: collisionShape(std::move(s)) {
	}

	Collider::~Collider() {}

	btCollisionShape& Collider::getBtShape() const {
		return *collisionShape;
	}
}