#pragma once
#include "Math/Vec3.h"
class btTransform;
class btRigidBody;

namespace Physics {
	class RigidBody {
	public:
		btRigidBody* body;

		RigidBody();
		RigidBody(float mass);

		void instantiate();
		void setPosition(Vector3f position);
	};
}

