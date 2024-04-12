#include "Physics/Own/Collision.h"
#include "Physics/Own/Collisions.h"
#include "Physics/Own/Capsule.h"
#include "Session.h"
#include "World.h"
#include "Player.h"
#include "Chunk.h"
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

typedef std::array<Vector3f, 3> Triangle;

// Center collider on position
void adjustCollider(Capsule& capsule, Vector3f& position) {
	capsule.base = Vector3f{ position.x, position.y, position.z };
	capsule.tip = Vector3f{ position.x, position.y + 1.65f, position.z };
}

void sortCloseTrianglesIndices(std::vector<int>& indices, std::vector<Vector3f>& verts, Vector3f& position) {
	// All triangles vector
	Triangle* triangles = (Triangle*) & verts[0];

	// Sort all triangles so closer ones are checked for collision first
	std::sort(indices.begin(), indices.end(), [&](int ia, int ib) {
		Triangle& a = triangles[ia];
		Triangle& b = triangles[ib];
		Vector3f centerA = (a[0] + a[1] + a[2]) / 3;
		Vector3f centerB = (b[0] + b[1] + b[2]) / 3;
		return (centerA - position).lengthSquared() > (centerB - position).lengthSquared();
	});
}

std::vector<Triangle> sortCloseTriangles(std::vector<Vector3f>& verts, Vector3f& position) {
	// All triangles vector
	Triangle* begin = (Triangle*)&verts[0];
	Triangle* end = (Triangle*)(&verts[0]) + verts.size() / 3;
	std::vector<Triangle> triangles{ begin, end };

	// Sort all triangles so closer ones are checked for collision first
	std::sort(triangles.begin(), triangles.end(), [&](const Triangle& a, const Triangle& b) {
		Vector3f centerA = (a[0] + a[1] + a[2]) / 3;
		Vector3f centerB = (b[0] + b[1] + b[2]) / 3;
		return (centerA - position).lengthSquared() > (centerB - position).lengthSquared();
	});
	return triangles;
}

void doPlayerPhysics(float timeDelta, Vector3f& position, Vector3f& velocity, Capsule& collider, Session& session) {
	auto capsuleCollider = &collider;
	auto& chunks = session.getWorld().chunks;

	// Calculate the next position the player would be in and recenter the colliders in the future position
	auto future = position + velocity * timeDelta;

	// For every triangle in the chunk mesh, check if there is a collision between the triangle, and the future collider
	for (auto& ch : chunks) {
		auto& verts = ch->vertices;
		if (verts.size() == 0) continue;

		// Chunk origin
		Vector3f origin{ ch->cx * 16.0f, 0, ch->cz * 16.0f };
		Vector3f shiftedFuture;

		// Discard chunk if it is too far away too matter
		if ((origin + Vector3f{ 8, 8, 8 } - position).lengthSquared() > 16 * 16) continue;

		// Calculate new future position (shifted by chunk origin)
		shiftedFuture = future - origin;

		// Fill triangle indices with sequential numbers starting from 0
		Triangle* triangles = (Triangle*)&verts[0];
		std::vector<int> triangleIndices;

		for (int i = 0; i < verts.size() / 3; i++) {
			// Quickly discard any triangle that is too far away to matter
			if ((triangles[i][0] - shiftedFuture).lengthSquared() > 8) continue;

			triangleIndices.push_back(i);
		}

		// Sort all triangles so closer ones are checked for collision first
		//auto triangles = sortCloseTriangles(verts, shiftedFuture);
		sortCloseTrianglesIndices(triangleIndices, verts, shiftedFuture);

		// Calculate shifted future position and center collider on that position.
		// Triangle vertices inside of chunks have coordinates relative to the chunk itself
		// Instead of shifting every triangle by the chunk origin, we shift the collider position
		// itself so that it becomes relative to the chunk
		adjustCollider(*capsuleCollider, shiftedFuture);

		for (int index : triangleIndices) {
			auto& triangle = triangles[index];
			
		//for (auto& triangle : triangles) {
			// Quickly discard any triangle that is too far away to matter
			//if ((triangle[0] - shiftedFuture).lengthSquared() > 8) continue;

			//session.getPlayer().collidingTriangles.emplace_back(Triangle{ triangle[0] + origin, triangle[1] + origin, triangle[2] + origin });


			// Perform collision detection
			auto coll = Collisions::doCapsuleTriangle(*capsuleCollider, triangle.data());
			if (!coll) continue;

			// There was a collision, save the triangle on a list
			session.getPlayer().collidingTriangles.emplace_back(Triangle{ triangle[0] + origin, triangle[1] + origin, triangle[2] + origin });

			// Use the collision information to adjust current velocity and the future position
			coll->correctMotion(future, velocity);

			// Recalculate shifted future and collider positions for the next checks
			shiftedFuture = future - origin;
			adjustCollider(*capsuleCollider, shiftedFuture);
		}
	}

	position = future;

	/*auto savedVelocity = velocity;
	velocity = { 0, -3, 0 };

	// Calculate the next position the player would be in and recenter the colliders in the future position
	future = position + velocity * timeDelta;

	// For every triangle in the chunk mesh, check if there is a collision between the triangle, and the future collider
	for (auto& ch : chunks) {
		auto& verts = ch->vertices;
		if (verts.size() == 0) continue;

		// Chunk origin
		Vector3f origin{ ch->cx * 16.0f, 0, ch->cz * 16.0f };
		Vector3f shiftedFuture;

		// All triangles vector
		Triangle* begin = (Triangle*)&verts[0];
		Triangle* end = (Triangle*)(&verts[0]) + verts.size() / 3;
		std::vector<Triangle> triangles{ begin, end };

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
			coll->correctGravityMotion(future, velocity);

			// Recalculate shifted future and collider positions for the next checks
			shiftedFuture = future - origin;
			capsuleCollider->base = Vector3f{ shiftedFuture.x, shiftedFuture.y, shiftedFuture.z };
			capsuleCollider->tip = Vector3f{ shiftedFuture.x, shiftedFuture.y + 1.65f, shiftedFuture.z };
		}
	}

	velocity = savedVelocity;
	position = future;*/
}
