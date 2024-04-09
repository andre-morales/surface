#include "Player.h"
#include "Session.h"
#include "World.h"
#include "Chunk.h"
#include "Physics/Collisions.h"
#include "Physics/AABB.h"
#include "Client.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Loggy.h"

static Loggy::Logger print{ "Player" };

Player::Player(Session& s) : session(s) {
	this->position = {10, 5, 10};
	collider = mkUnique<AABB>();
	collider->extents = Vector3f{ .3, .825, .3 };
}

void Player::doPhysics(float timeDelta) {
	auto& chunks = session.getWorld().chunks;
	collidingTriangles.clear();

	// Calculate the next position the player would be in and create an AABB that represents
	// the collider of the future position
	auto nextPosition = position + velocity * timeDelta;
	collider->center = Vector3f{ nextPosition.x, nextPosition.y + 0.825f, nextPosition.z };
	
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
		
			// If there was no collision, keep on looking
			if (!Collisions::isIntersecting(*collider, triangle.data())) continue;

			// There was a collision, save the triangle on a list
			collidingTriangles.push_back(triangle);

			// Get infinite plane from triangle
			auto trianglePlane = triangle;
			Plane plane;
			plane.fromTriangle(trianglePlane[0], trianglePlane[1], trianglePlane[2]);

			// Get triangle normal
			auto normal = plane.direction;
			
			// Sliding direction for this triangle, with all the opposing speed removed
			auto newDir = Vector3f::cross(Vector3f::cross(normal, velocity), normal);
			if (std::abs(newDir.x) < .05) newDir.x = 0;
			if (std::abs(newDir.y) < .05) newDir.y = 0;
			if (std::abs(newDir.z) < .05) newDir.z = 0;
			velocity = newDir;

			// Recalculate the future position and recenter AABB for the next collision checks
			nextPosition = position + velocity * timeDelta;
			collider->center = Vector3f{ nextPosition.x, nextPosition.y + 0.825f, nextPosition.z };
		}
	}

	position = nextPosition;
}

std::array<Vector3f, 24> Player::getColliderBoxLines() {
	auto boxv = collider->getVertices();

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