#include "Player.h"
#include "Session.h"
#include "World.h"
#include "Chunk.h"
#include "Physics/Collisions.h"
#include "Physics/AABB.h"
#include "Physics/Capsule.h"
#include "Client.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Loggy.h"

static Loggy::Logger print{ "Player" };

Player::Player(Session& s) : session(s) {
	this->position = {10, 5, 10};
	boxCollider = mkUnique<AABB>();
	boxCollider->extents = Vector3f{ .5, .825, .5 };

	capsuleCollider = mkUnique<Capsule>();
	capsuleCollider->radius = .4f;
}

Player::~Player() {}

void Player::doPhysics(float timeDelta) {
	auto& chunks = session.getWorld().chunks;
	collidingTriangles.clear();

	auto originalNextPosition = position + velocity * timeDelta;
	auto originalVelocity = velocity;

	// Calculate the next position the player would be in and recenter the colliders in the future position
	auto futurePos = position + velocity * timeDelta;
	boxCollider->center = Vector3f{ futurePos.x, futurePos.y + 0.825f, futurePos.z };
	capsuleCollider->base = Vector3f{ futurePos.x, futurePos.y, futurePos.z };
	capsuleCollider->tip = Vector3f{ futurePos.x, futurePos.y + 1.65f, futurePos.z };

	// For every triangle in the chunk mesh, check if there is a collision between the triangle, and the future AABB
	for (auto& ch : chunks) {
		auto& verts = ch->vertices;
		Vector3f origin{ch->cx * 16.0f, 0, ch->cz * 16.0f};
		for (int i = 0; i < verts.size(); i += 3) {
			std::array triangle {
				verts[i + 0] + origin,
				verts[i + 1] + origin,
				verts[i + 2] + origin
			};
			
			// Quickly discard any triangle that is too far away to matter
			if ((triangle[0] - futurePos).lengthSquared() > 16) continue;

			// Perform collision detection
			auto coll = Collisions::doCapsuleTriangle(*capsuleCollider, triangle.data());
			if (!coll) continue;

			// There was a collision, save the triangle on a list
			collidingTriangles.push_back(triangle);

			// ---- Clip using collision information as implemented by wicked engine, way smoother
			auto normal = coll->normal;
			auto penetration = coll->penetration;

			auto nVelocity = velocity.normalize();
			auto velocityM = velocity.length();
			auto undesired = normal * Vector3f::dot(nVelocity, normal);
			auto desired = nVelocity - undesired;
			velocity = desired * velocityM;

			// Readjust future position and recenter capsule collider
			futurePos += normal * (penetration + 0.00001f);
			capsuleCollider->base = Vector3f{ futurePos.x, futurePos.y, futurePos.z };
			capsuleCollider->tip = Vector3f{ futurePos.x, futurePos.y + 1.65f, futurePos.z }; 
		}
	}

	position = futurePos;

	// If we are ignoring physics, do not alter velocity or position according to collisions
	if (!session.noClipping) {
		velocity = originalVelocity;
		position = originalNextPosition;
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