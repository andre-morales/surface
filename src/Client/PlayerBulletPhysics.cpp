#include "Physics/Physics.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace Physics {
	static btRigidBody* playerBody;
	static btRigidBody* ground;

	Vector3f getPlayerPosition() {
		auto vec = playerBody->getWorldTransform().getOrigin();
		return { vec.x(), vec.y(), vec.z() };
	}

	void initBulletWorldPhysics() {
		// Create player rigid body and capsule collider
		{
			btScalar mass = 1;
			btTransform transform{};
			transform.setIdentity();
			transform.setOrigin({ 10, 25, 0 });
			auto motionState = new btDefaultMotionState(transform);

			btVector3 inertia{ 0, 0, 0 };

			auto capsule = new btCapsuleShape(1, 2);
			capsule->calculateLocalInertia(mass, inertia);

			btRigidBody::btRigidBodyConstructionInfo bodyInfo{ mass, motionState, capsule, inertia };
			playerBody = new btRigidBody(bodyInfo);
			playerBody->setFriction(1);

			_world->addRigidBody(playerBody);
		}

		// Create ground shape
		{
			//auto collisionShape = new btBoxShape(btVector3{ 100.0f, 5.0f, 100.0f });
			auto collisionShape = new btStaticPlaneShape({ 0, 1, 0 }, 1);

			btTransform transform{};
			transform.setIdentity();
			transform.setOrigin({ 0, 5, 0 });

			auto motionState = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo bodyInfo{ 0, motionState, collisionShape};

			ground = new btRigidBody(bodyInfo);

			_world->addRigidBody(ground);
		}
		
	}
}