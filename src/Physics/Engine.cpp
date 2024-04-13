#include "Engine.h"
#include "VisualDebugger.h"
#include <bullet/btBulletDynamicsCommon.h>

static Vector3f convert(const btVector3 v) {
	return { v.x(), v.y(), v.z() };
}

namespace Physics {
	btDiscreteDynamicsWorld* _world;

	void initWorld() {
		// Instantiate a Dynamic AABB tree broadphase physics implementation
		auto broadphase = new btDbvtBroadphase();

		// Instantiate fine-grained collision handler with default configs
		auto collisionConfig = new btDefaultCollisionConfiguration();
		auto dispatcher = new btCollisionDispatcher(collisionConfig);

		auto solver = new btSequentialImpulseConstraintSolver();

		// Instantiate physics world
		_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
		_world->setGravity({ 0, -9.8, 0 });
		
		printf("World physics intialized.\n");
	}

	void simulate(float timeDelta) {
		_world->stepSimulation(timeDelta);
	}

	void enableDebugging() {
		auto debugger = new VisualDebugger();
		debugger->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
		_world->setDebugDrawer(debugger);
	}

	void drawDebug() {
		_world->debugDrawWorld();
	}
}