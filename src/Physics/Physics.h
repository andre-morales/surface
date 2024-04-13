#pragma once
#include "Math/Vec3.h"
class btDiscreteDynamicsWorld;
class Chunk;

namespace Physics {
	void initWorld();
	void simulate(float timeDelta);
	void drawDebug();

	void initBulletWorldPhysics();
	Vector3f getPlayerPosition();
	void setPlayerMotion(Vector3f v);
	void createChunkCollider(Chunk* c);

	extern btDiscreteDynamicsWorld* _world;
};

