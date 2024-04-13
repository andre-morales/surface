#pragma once
#include "Math/Vec3.h"
class btDiscreteDynamicsWorld;
class Chunk;

namespace Physics {
	// -- Engine functions --
	void initWorld();
	void simulate(float timeDelta);
	void enableDebugging();
	void drawDebug();

	// -- Engine bridge --
	void initBulletWorldPhysics();
	Vector3f getPlayerPosition();
	void setPlayerMotion(Vector3f v);
	void createChunkCollider(Chunk* c);
	void destroyChunkCollider(Chunk* c);

	extern btDiscreteDynamicsWorld* _world;
};

