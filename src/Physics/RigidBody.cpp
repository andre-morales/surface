#include "RigidBody.h"
#include "Engine.h"
#include "Collider.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace Physics {
	RigidBody::RigidBody() 
		: RigidBody(0){}

	RigidBody::RigidBody(float mass) {
		body.reset(new btRigidBody{mass, nullptr, nullptr});
		body->setAngularFactor({ 0, 0, 0 }); // Disable rotation of any kind
	}

	RigidBody::~RigidBody() {
		Physics::_world->removeRigidBody(body.get());
		body.release();
		collider.release();
	}

	void RigidBody::instantiate() {
		Physics::_world->addRigidBody(body.get());
	}

	void RigidBody::setCollider(unique<Collider>&& coll) {
		body->setCollisionShape(&coll->getBtShape());
		collider = std::move(coll);
	}

	void RigidBody::setPosition(Vector3f position) {
		btTransform transform = body->getWorldTransform();
		transform.setOrigin({ position.x, position.y, position.z });
		body->setWorldTransform(transform);
	}
	
	btRigidBody& RigidBody::getBtBody() const {
		return *body;
	}
}