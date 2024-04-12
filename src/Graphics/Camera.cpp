#include "Camera.h"
#include "Client/Client.h"
#include "Glow/Window.h"
#include "Client/Chunk.h"
#include "Client/Session.h"
#include "Physics/Own/Ray.h"
#include "Physics/Own/Collisions.h"
#include "Math/Maths.h"
#include "Client/World.h"

#define RADS (3.14159265359/180.0)

Camera::Camera() {}

void Camera::makeTransformMatrices() {
	projectionMat = Math::Mat4f::perspective(aspect, fov, near, far);
	rotatedViewMat = Math::Mat4f::rotationY(rotation.y * RADS) * Math::Mat4f::rotationX(rotation.x * RADS);
	modelViewMat = Math::Mat4f::translation(-position.x, -position.y, -position.z) * rotatedViewMat;
	fullTransformMat = modelViewMat * projectionMat;
}

void Camera::doCameraRaycast() {
	Client& client = Client::get();
	Session& session = *client.getSession();

	// Ray positioned at camera center
	Ray ray;
	ray.position = position;
	ray.direction = getLookingDirection();

	// Variables to keep track closest hit triangle
	float closestDistance = std::numeric_limits<float>::max();
	bool didHit = false;
	Vector3f closestHit;
	std::array<Vector3f, 3> closestTriangle;

	for (Chunk* chunk : session.getWorld().chunks) {
		// Camera position relative to chunk origin corner
		Vector3f chunkOrigin{ chunk->cx * 16.0f, 0, chunk->cz * 16.0f };
		Vector3f shiftedPosition = position - chunkOrigin;

		auto& verts = chunk->vertices;
		auto vertsSize = verts.size();

		for (int i = 0; i < vertsSize; i += 3) {
			const float MAX_DISTANCE = 4;

			if (abs(shiftedPosition.x - verts[i].x) > MAX_DISTANCE) continue;
			if (abs(shiftedPosition.z - verts[i].z) > MAX_DISTANCE) continue;
			if (abs(shiftedPosition.y - verts[i].y) > MAX_DISTANCE) continue;

			std::array<Vector3f, 3> triangle{
				verts[i + 0] + chunkOrigin,
				verts[i + 1] + chunkOrigin,
				verts[i + 2] + chunkOrigin
			};

			auto hit = Collisions::raycastTriangle(ray, triangle.data());
			if (hit) {
				float len = (position - *hit).lengthSquared();
				if (len < closestDistance) {
					closestDistance = len;
					closestHit = *hit;
					closestTriangle = triangle;
					didHit = true;
				}
			}
		}
	}
	if (!didHit) {
		isLookingAtTerrain = false;
		return;
	}

	isLookingAtTerrain = true;
	lookingTerrainPos = closestHit;
	lookingTerrainTriangle = closestTriangle;
}

Vector3f Camera::getLookingDirection() {
	float siny = sin(rotation.y * RADS);
	float cosy = cos(rotation.y * RADS);
	float sinx = sin(rotation.x * RADS);
	float cosx = cos(rotation.x * RADS);

	return { siny * cosx , -sinx, -cosy * cosx };
}

std::optional<Vector3f> Camera::getLookingWorldPos() {
	if (!isLookingAtTerrain) return {};
	
	return lookingTerrainPos;
}

std::optional<Vector3i> Camera::getLookingBlockPos() {
	Client& client = Client::get();
	Session& session = *client.getSession();
	World& world = session.getWorld();

	auto pos = getLookingWorldPos();
	if (!pos) return {};

	auto hit = *pos;
	hit += Vector3f(0.5f);

	Vector3i hitI{ (int)floor(hit.x), (int)floor(hit.y), (int)floor(hit.z) };
	auto [x, y, z] = hitI.vec;

	if (world.getBlockAt(x, y, z) == 0) {
		auto lookdir = getLookingDirection();

		float xr = abs( ( std::ceil(hit.x) - hit.x ) / lookdir.x);
		float yr = abs( ( std::ceil(hit.y) - hit.y ) / lookdir.y);
		float zr = abs( ( std::ceil(hit.z) - hit.z ) / lookdir.z);

		if (xr < yr && xr < zr) {
			x += lookdir.x / abs(lookdir.x) ;
		}
		if (yr < xr && yr < zr) {
			y += lookdir.y / abs(lookdir.y);
		}
		if (zr < xr && zr < yr) {
			z += lookdir.z / abs(lookdir.z);
		}

	}

	return { {x, y, z} };
}

std::optional<Vector3i> Camera::getLookingEmptyBlockPos() {
	Client& client = Client::get();
	Session& session = *client.getSession();
	World& world = session.getWorld();

	if (!isLookingAtTerrain) {
		return {};
	}

	auto hit = lookingTerrainPos;
	auto& triangle = lookingTerrainTriangle;

	hit += Vector3f(0.5f);

	Vector3i hitI{ (int)floor(hit.x), (int)floor(hit.y), (int)floor(hit.z) };

	auto [x, y, z] = hitI.vec;
	if (world.getBlockAt(x, y, z) != 0) {
		Vector3f triangleDir;
		float distance;
		Math::planeFromTriangle(triangle.data(), &triangleDir, &distance);

		auto lookDir = -getLookingDirection();

		auto dir = (triangleDir + lookDir);

		float xr = abs((std::ceil(hit.x) - hit.x) / dir.x);
		float yr = abs((std::ceil(hit.y) - hit.y) / dir.y);
		float zr = abs((std::ceil(hit.z) - hit.z) / dir.z);

		if (xr < yr && xr < zr) {
			x += dir.x / abs(dir.x);
		}
		if (yr < xr && yr < zr) {
			y += dir.y / abs(dir.y);
		}
		if (zr < xr && zr < yr) {
			z += dir.z / abs(dir.z);
		}

	}

	return { {x, y, z} };
}