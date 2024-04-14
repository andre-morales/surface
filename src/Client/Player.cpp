#include "Player.h"
#include "Session.h"
#include "World.h"
#include "Chunk.h"
#include "Physics/Own/Collisions.h"
#include "Physics/Engine.h"
#include "Client.h"
#include "Camera.h"
#include "Graphics/Renderer.h"
#include "Loggy.h"
#include <algorithm>

typedef std::array<Vector3f, 3> Triangle;

static Loggy::Logger print{ "Player" };

Player::Player(Session& s) : session(s) {
	this->position = {10, 5, 10};
}

Player::~Player() {}

void Player::doPhysics(float timeDelta) {
	auto& chunks = session.getWorld().chunks;

	// Update player position and motion forces
	position = Physics::getPlayerPosition() - Vector3f{ 0, 2, 0 };
	Physics::setPlayerMotion(motion);
}
