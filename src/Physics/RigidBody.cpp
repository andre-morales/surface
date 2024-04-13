#include "RigidBody.h"
#include "Engine.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace Physics {
	RigidBody::RigidBody() 
		: RigidBody(0){}

	RigidBody::RigidBody(float mass) {
		body = new btRigidBody{ mass, nullptr, nullptr };
		body->setAngularFactor({ 0, 0, 0 }); // Disable rotation of any kind
	}

	void RigidBody::setPosition(Vector3f position) {
		btTransform transform = body->getWorldTransform();
		transform.setOrigin({ position.x, position.y, position.z });
		body->setWorldTransform(transform);
	}

	void RigidBody::instantiate() {
		Physics::_world->addRigidBody(body);
	}
}