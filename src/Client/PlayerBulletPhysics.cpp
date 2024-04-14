#include "Physics/Engine.h"
#include "Client/Chunk.h"
#include "Physics/RigidBody.h"
#include "Physics/Collider.h"
#include "Physics/MeshCollider.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <unordered_map>

btVector3 convertB(const Vector3f v) {
	return { v.x, v.y, v.z };
}

Vector3f convertV(const btVector3 v) {
	return { v.x(), v.y(), v.z() };
}

namespace Physics {
	class BatchedChunk {
	public:
		unique<RigidBody> rigidBody;
	};

	static btRigidBody* playerBody;
	static btRigidBody* ground;
	static std::unordered_map<Chunk*, BatchedChunk> chunks;

	Vector3f getPlayerPosition() {
		auto vec = playerBody->getWorldTransform().getOrigin();
		return { vec.x(), vec.y(), vec.z() };
	}

	void setPlayerMotion(Vector3f v) {
		playerBody->applyCentralForce(convertB(v * 3));
		/*
		auto vel = playerBody->getLinearVelocity();
		if (vel.length() > 5) {
			vel /= vel.length() / 5;
			//playerBody->setLinearVelocity(vel);
			
			
		}*/
	}

	void destroyChunkCollider(Chunk* c) {
		chunks.erase(c);
	}

	void createChunkCollider(Chunk* c) {
		BatchedChunk* chp = nullptr;
		auto it = chunks.find(c);
		bool newChunk;
		if (it != chunks.end()) {
			chp = &it->second;
			newChunk = false;
		} else {
			auto it_ = chunks.emplace(c, BatchedChunk{});
			chp = &it_.first->second;
			newChunk = true;
		}

		BatchedChunk& ch = *chp;
		auto meshP = mkUnique<btTriangleMesh>();
		auto& mesh = *meshP;

		for (int i = 0; i < c->vertices.size(); i += 3) {
			mesh.addTriangle(convertB(c->vertices[i]), convertB(c->vertices[i + 1]), convertB(c->vertices[i + 2]));
		}
		
		auto collider = mkUnique<MeshCollider>(std::move(meshP));

		if (newChunk) {
			ch.rigidBody = mkUnique<RigidBody>(0);
			ch.rigidBody->setPosition({ c->cx * 16.0f, 0, c->cz * 16.0f });
			ch.rigidBody->getBtBody().setFriction(1.8);
		}
		
		ch.rigidBody->setCollider(std::move(collider));
		if (newChunk) {
			ch.rigidBody->instantiate();
		}
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
			transform.setOrigin({ 10, 20, 0 });

			auto rb = new RigidBody(1);
			playerBody = &rb->getBtBody();
			playerBody->setActivationState(DISABLE_DEACTIVATION);
			
			auto capsule = mkUnique<btCapsuleShape>(0.4, 1.65 -0.4*2);
			auto collider = mkUnique<Collider>(std::move(capsule));

			rb->setCollider(std::move(collider));
			rb->setPosition({ 10, 30, 0 });
			rb->instantiate();
		}

		// Create ground shape
		{
			auto collisionShape = new btStaticPlaneShape({ 0, 1, 0 }, 1);

			btTransform transform{};
			transform.setIdentity();
			transform.setOrigin({ 0, -10, 0 });

			auto motionState = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo bodyInfo{ 0, motionState, collisionShape};

			ground = new btRigidBody(bodyInfo);
			ground->setFriction(1);

			_world->addRigidBody(ground);
		}
		
	}
}