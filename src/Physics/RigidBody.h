#pragma once
#include "Math/Vec3.h"
#include "Pointers.h"

class btTransform;
class btRigidBody;

namespace Physics {
	class Collider;

	class RigidBody {
	public:
		RigidBody();
		RigidBody(float mass);
		~RigidBody();

		void instantiate();
		void setCollider(unique<Collider>&& collider);
		void setPosition(Vector3f position);
		btRigidBody& getBtBody() const;

	private:
		unique<Collider> collider;
		unique<btRigidBody> body;

		RigidBody(RigidBody&&) = delete;
		RigidBody(const RigidBody&) = delete;
	};
}

