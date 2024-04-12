#pragma once
#include "Math/Vec3.h"
class btDiscreteDynamicsWorld;

namespace Physics {
	void initWorld();
	void initBulletWorldPhysics();

	void simulate(float timeDelta);
	Vector3f getPlayerPosition();
	
	void drawDebug();

	extern btDiscreteDynamicsWorld* _world;
};

