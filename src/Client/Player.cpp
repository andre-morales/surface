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
	this->position = {13.51f, 3.07f, -0.22f};
	collider = mkUnique<AABB>();
	collider->extents = Vector3f{ .3, .825, .3 };
}

// TODO: Make physics work
void Player::doPhysics(float timeDelta) {
	auto& chunks = session.getWorld().chunks;
	auto& camera = Client::get().getRenderer()->camera;
	collidingTriangles.clear();

	// Calculate the next position the player would be in
	auto nextPosition = position + velocity * timeDelta;
	
	// Reposition the ABB in the center of the player box
	collider->center = Vector3f{ nextPosition.x, nextPosition.y + 0.825f, nextPosition.z };
	
	if (chunks.size() == 0) return;
	
	int collisions = 0;

	// For every triangle in the chunk mesh, check if there is a collision between the triangle, and the future AABB
	auto& verts = chunks[0]->vertices;
	Vector3f sum{};
	for (int i = 0; i < verts.size(); i += 3) {
		std::array triangle {
			verts[i + 0],
			verts[i + 1],
			verts[i + 2]
		};

		// If there is a collision, save it in the collision vector, and keep looking for more
		if (Collisions::isIntersecting(*collider, triangle.data())) {
			collidingTriangles.push_back(triangle);

			//collisions++;

			// Get infinite plane from triangle
			auto trianglePlane = triangle;
			Plane plane;
			plane.fromTriangle(trianglePlane[0], trianglePlane[1], trianglePlane[2]);

			// Get triangle normal
			auto normal = plane.direction;
			
			// Sliding direction for this triangle, cancels all the speed in the direction opposing this triangle
			auto dir = Vector3f::cross(Vector3f::cross(normal, velocity), normal);

			velocity = dir;
			nextPosition = position + velocity * timeDelta;

			// Reposition the ABB in the center of the player box
			collider->center = Vector3f{ nextPosition.x, nextPosition.y + 0.825f, nextPosition.z };
			//print("D: ", dir.toString());
			//sum += dir;

		}
	}

	// Only advance to the future position if no collision happened
	//if (!collisions) {
		position = nextPosition;
	//	return;
	//}
	//velocity = sum / collisions;
	//position += sum * timeDelta;
	/*// Get infinite plane from triangle
	auto trianglePlane = collidingTriangles[0];
	Plane plane;g\\
	plane.fromTriangle(trianglePlane[0], trianglePlane[1], trianglePlane[2]);

	// Get triangle normal
	auto normal = plane.direction;

	auto dir = Vector3f::cross(Vector3f::cross(normal, velocity), normal);*/

	//velocity = {};
	//position += velocity * timeDelta;
	//print("S: ", sum.x, ", ", sum.y, ",", sum.z);
	//print("V: ", velocity.x, ", ", velocity.y, ",", velocity.z);
	//print("D: ", dir.x, ", ", dir.y, ",", dir.z);

	//velocity = {};
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