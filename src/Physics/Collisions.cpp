#include "Collisions.h"
#include <limits>
#include <span>
#include "Capsule.h"
#include "Loggy.h"

static Loggy::Logger print{ "Collisions" };

std::optional<Vector3f> Collisions::raycastTriangle(const Ray& ray, const Vector3f triangle[]) {
	// Create infinite plane from triangle
	Plane plane;
	{
		const auto& vertA = triangle[0];
		const auto& vertB = triangle[1];
		const auto& vertC = triangle[2];

		plane.direction = Vector3f::cross(vertB - vertA, vertC - vertA).normalize();
		plane.distance = Vector3f::dot(plane.direction, vertA);
	}

	// Raycast against infinite plane
	auto hit = raycastPlaneNoNormal(ray, plane);
	if (!hit) return {};

	// If it dit hit the infinite plane, check if the hit is inside the triangle
	if (isPointInTriangle(*hit, triangle)) {
		return hit;
	}

	return {};
}

std::optional<Vector3f> Collisions::raycastPlaneNoNormal(const Ray& ray, const Plane& plane) {
	float nd = Vector3f::dot(ray.direction, plane.direction);

	// If the dot product is too close to 0, the ray and the plane are pretty much parallel
	// and will collide only really far away.
	if (abs(nd) < 0.001) {
		return {};
	}

	float pn = Vector3f::dot(ray.position, plane.direction);
	float t = (plane.distance - pn) / nd;
	if (t >= 0) {
		return { ray.position + ray.direction * t };
	}

	return {};
}

bool Collisions::isPointInTriangle(const Vector3f& point, const Vector3f triangle[]) {
	// Move the triangle so that the point becomes the 
	// triangles origin
	auto a = triangle[0] - point;
	auto b = triangle[1] - point;
	auto c = triangle[2] - point;

	// Compute the normal vectors for triangles:
	// u = normal of PBC
	// v = normal of PCA
	// w = normal of PAB
	Vector3f u = Vector3f::cross(b, c);
	Vector3f v = Vector3f::cross(c, a);
	Vector3f w = Vector3f::cross(a, b);

	// Test to see if the normals are facing 
	// the same direction, return false if not
	if (Vector3f::dot(u, v) < 0.0f) return false;
	if (Vector3f::dot(u, w) < 0.0f) return false;

	// All normals facing the same way, return true
	return true;
}
