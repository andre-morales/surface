#include "Player.h"
#include "Session.h"
#include "World.h"
#include "Chunk.h"
#include "Physics/Own/Collisions.h"
#include "Physics/Own/AABB.h"
#include "Physics/Own/Capsule.h"
#include "Physics/Physics.h"
#include "Client.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Loggy.h"
#include <algorithm>

typedef std::array<Vector3f, 3> Triangle;

static Loggy::Logger print{ "Player" };

Player::Player(Session& s) : session(s) {
	this->position = {10, 5, 10};
	boxCollider = mkUnique<AABB>();
	boxCollider->extents = Vector3f{ .5, .825, .5 };

	capsuleCollider = mkUnique<Capsule>();
	capsuleCollider->radius = .3f;
}

Player::~Player() {}

void Player::initWorldPhysics() {
	Physics::initBulletWorldPhysics();
}

void Player::doWorldPhysics() {
	position = Physics::getPlayerPosition() - Vector3f{0, 2, 0};
	if (!session.noClipping) {
		Physics::setPlayerMotion(motion * 0.05);
	}
}

void doPlayerPhysics(float timeDelta, Vector3f& position, Vector3f& velocity, Capsule& collider, Session& session);
void Player::doPhysics(float timeDelta) {
	auto& chunks = session.getWorld().chunks;
	collidingTriangles.clear();

	// Calculated position and velocity before clipping
	auto originalFuture = position + velocity * timeDelta;
	auto originalVelocity = velocity;

	doWorldPhysics();

	//doPlayerPhysics(timeDelta, position, velocity, *capsuleCollider, session);

	// If we are ignoring physics, do not alter velocity or position according to collisions
	if (session.noClipping) {
		velocity = originalVelocity;
		position = originalFuture;
		return;
	}
}

std::array<Vector3f, 24> Player::getBoxColliderLines() {
	auto boxv = boxCollider->getVertices();

	return {
		boxv[0], boxv[1],
		boxv[0], boxv[2],
		boxv[0], boxv[4],
		boxv[1], boxv[3],
		boxv[1], boxv[5],
		boxv[2], boxv[3],
		boxv[2], boxv[6],
		boxv[3], boxv[7],
		boxv[4], boxv[5],
		boxv[4], boxv[6],
		boxv[5], boxv[7],
		boxv[6], boxv[7],
	};
}