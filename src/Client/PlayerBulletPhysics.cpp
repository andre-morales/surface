#include "Physics/Physics.h"
#include "Client/Chunk.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "Physics/RigidBody.h"

btVector3 convertB(const Vector3f v) {
	return { v.x, v.y, v.z };
}

Vector3f convertV(const btVector3 v) {
	return { v.x(), v.y(), v.z() };
}

namespace Physics {
	static btRigidBody* playerBody;
	static btRigidBody* ground;

	Vector3f getPlayerPosition() {
		auto vec = playerBody->getWorldTransform().getOrigin();
		return { vec.x(), vec.y(), vec.z() };
	}

	void setPlayerMotion(Vector3f v) {
		playerBody->applyCentralImpulse(convertB(v));
		//playerBody->setLinearVelocity(convertB(v));
	}

	void createChunkCollider(Chunk* c) {
		auto mesh = new btTriangleMesh();
		for (int i = 0; i < c->vertices.size(); i += 3) {
			mesh->addTriangle(convertB(c->vertices[i]), convertB(c->vertices[i + 1]), convertB(c->vertices[i + 2]));
		}
		
		auto collisionShape = new btBvhTriangleMeshShape(mesh, true);

		btTransform transform{};
		transform.setIdentity();
		transform.setOrigin({ c->cx * 16.0f, 0, c->cz * 16.0f });

		auto motionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo bodyInfo{ 0, motionState, collisionShape };

		auto chunkBody = new btRigidBody(bodyInfo);
		_world->addRigidBody(chunkBody);
	}

	void initBulletWorldPhysics() {
		// Create player rigid body and capsule collider
		{
			

			/*btScalar mass = 1;
			btTransform transform{};
			transform.setIdentity();
			transform.setOrigin({ 10, 30, 0 });
			auto motionState = new btDefaultMotionState(transform);

			btVector3 inertia{ 0, 0, 0 };

			auto capsule = new btCapsuleShape(1, 2);
			capsule->calculateLocalInertia(mass, inertia);
			
			btRigidBody::btRigidBodyConstructionInfo bodyInfo{ mass, nullptr, nullptr, inertia };
			playerBody = new btRigidBody(bodyInfo);
			playerBody->setCollisionShape(capsule);
			playerBody->setWorldTransform(transform);
			playerBody->setAngularFactor({ 0, 0, 0 }); // Disable rotation of any kind

			_world->addRigidBody(playerBody);*/

			btTransform transform{};
			transform.setIdentity();
			transform.setOrigin({ 10, 30, 0 });

			auto rb = new RigidBody(1);
			playerBody = rb->body;
			playerBody->setActivationState(DISABLE_DEACTIVATION);
			auto capsule = new btCapsuleShape(1, 2);
			rb->body->setCollisionShape(capsule);
			rb->setPosition({ 10, 30, 0 });
			rb->instantiate();
		}

		// Create ground shape
		{
			/*//auto collisionShape = new btBoxShape(btVector3{ 100.0f, 5.0f, 100.0f });
			auto collisionShape = new btStaticPlaneShape({ 0, -10, 0 }, 1);

			btTransform transform{};
			transform.setIdentity();
			transform.setOrigin({ 0, 15, 0 });

			auto motionState = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo bodyInfo{ 0, motionState, collisionShape};

			ground = new btRigidBody(bodyInfo);

			_world->addRigidBody(ground);*/
		}
		
	}
}