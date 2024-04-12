#include "Physics.h"
#include "Graphics/Renderer.h"
#include <bullet/btBulletDynamicsCommon.h>

Vector3f convert(const btVector3 v) {
	return { v.x(), v.y(), v.z() };
}

class BulletDebugBridge : public btIDebugDraw {
public:
	int debugMode;

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
		Renderer::get().drawLine(convert(from), convert(to), convert(color));
	}

	void setDebugMode(int mode) override {
		this->debugMode = mode;
	}

	int getDebugMode() const override {
		return debugMode;
	}

	void reportErrorWarning(const char* msg) override {
		printf("[BULLET] %s\n", msg);
	}

	void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override {}
	void draw3dText(const btVector3&, const char*) override {}
};

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

		auto debugger = new BulletDebugBridge();
		debugger->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);

		_world->setDebugDrawer(debugger);
		printf("World physics intialized.\n");
	}

	void simulate(float timeDelta) {
		//printf("Simulated.\n");
		
		_world->stepSimulation(timeDelta);
	}

	void drawDebug() {
		_world->debugDrawWorld();
	}
}