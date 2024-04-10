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
#include <algorithm>

typedef std::array<Vector3f, 3> Triangle;

static Loggy::Logger print{ "Player" };

Player::Player(Session& s) : session(s) {
	this->position = {10, 5, 10};
	boxCollider = mkUnique<AABB>();
	boxCollider->extents = Vector3f{ .5, .825, .5 };

	capsuleCollider = mkUnique<Capsule>();
	capsuleCollider->radius = .4f;
}

Player::~Player() {}

// ---- Clip using collision information as implemented by wicked engine, way smoother
void correctCollision(const Collision& coll, Vector3f& velocity, Vector3f& position) {
	auto normal = coll.normal;
	auto penetration = coll.penetration;

	auto nVelocity = velocity.normalize();
	auto velocityM = velocity.length();
	auto undesired = normal * Vector3f::dot(nVelocity, normal);
	auto desired = nVelocity - undesired;
	velocity = desired * velocityM;

	// Readjust future position and recenter capsule collider
	position += normal * (penetration + 0.00001f);
}

void Player::doPhysics(float timeDelta) {
	auto& chunks = session.getWorld().chunks;
	collidingTriangles.clear();

	// Calculated position and velocity before clipping
	auto originalFuture = position + velocity * timeDelta;
	auto originalVelocity = velocity;

	// Calculate the next position the player would be in and recenter the colliders in the future position
	auto future = position + velocity * timeDelta;

	// For every triangle in the chunk mesh, check if there is a collision between the triangle, and the future AABB
	for (auto& ch : chunks) {
		auto& verts = ch->vertices;
		if (verts.size() == 0) continue;

		// Chunk origin
		Vector3f origin{ ch->cx * 16.0f, 0, ch->cz * 16.0f };
		Vector3f shiftedFuture;

		// All triangles vector
		Triangle* begin = (Triangle*)&verts[0];
		Triangle* end = (Triangle*)(&verts[0]) + verts.size() / 3;
		std::vector<Triangle> triangles{begin, end};

		// Sort all triangles so closer ones are checked for collision first
		std::sort(triangles.begin(), triangles.end(), [&](const Triangle& a, const Triangle& b) {
			Vector3f centerA = (a[0] + a[1] + a[2]) / 3;
			Vector3f centerB = (b[0] + b[1] + b[2]) / 3;
			return (centerA - shiftedFuture).lengthSquared() > (centerB - shiftedFuture).lengthSquared();
		});

		// Calculate shifted future position and center collider on that position.
		// Triangle vertices inside of chunks have coordinates relative to the chunk itself
		// Instead of shifting every triangle by the chunk origin, we shift the collider position
		// itself so that it becomes relative to the chunk
		shiftedFuture = future - origin;
		capsuleCollider->base = Vector3f{ shiftedFuture.x, shiftedFuture.y, shiftedFuture.z };
		capsuleCollider->tip = Vector3f{ shiftedFuture.x, shiftedFuture.y + 1.65f, shiftedFuture.z };

		for (auto& triangle : triangles) {
			// Quickly discard any triangle that is too far away to matter
			if ((triangle[0] - shiftedFuture).lengthSquared() > 8) continue;

			// Perform collision detection
			auto coll = Collisions::doCapsuleTriangle(*capsuleCollider, triangle.data());
			if (!coll) continue;

			// There was a collision, save the triangle on a list
			collidingTriangles.emplace_back(Triangle{ triangle[0] + origin, triangle[1] + origin, triangle[2] + origin });
			
			// Use the collision information to adjust current velocity and the future position
			correctCollision(*coll, velocity, future);

			// Recalculate shifted future and collider positions for the next checks
			shiftedFuture = future - origin;
			capsuleCollider->base = Vector3f{ shiftedFuture.x, shiftedFuture.y, shiftedFuture.z };
			capsuleCollider->tip = Vector3f{ shiftedFuture.x, shiftedFuture.y + 1.65f, shiftedFuture.z };
		}
	}

	position = future;

	// If we are ignoring physics, do not alter velocity or position according to collisions
	if (!session.noClipping) {
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