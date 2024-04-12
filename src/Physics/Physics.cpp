#include "Physics.h"
#include <bullet/btBulletDynamicsCommon.h>

static btDiscreteDynamicsWorld* world;

void Physics::initWorld() {
	// Instantiate a Dynamic AABB tree broadphase physics implementation
	auto broadphase = new btDbvtBroadphase();

	// Instantiate fine-grained collision handler with default configs
	auto collisionConfig = new btDefaultCollisionConfiguration();
	auto dispatcher = new btCollisionDispatcher(collisionConfig);

	auto solver = new btSequentialImpulseConstraintSolver();

	// Instantiate physics world
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity({ 0, -9.8, 0 });

}