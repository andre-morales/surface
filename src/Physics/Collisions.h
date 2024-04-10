#pragma once
#include "Ray.h"
#include "Plane.h"
#include "AABB.h"
#include "Collision.h"
#include <optional>

class Capsule;

class Collisions {
public:
	static std::optional<Vector3f> raycastTriangle(const Ray& ray, const Vector3f triangle[]);
    static bool isIntersecting(const AABB& box, const Vector3f triangle[]);
	static std::optional<Collision> isIntersecting(const Capsule& caps, const Vector3f triangle[]);
	static std::optional<Collision> triangleIntersectsSphere(const Vector3f triangle[], Vector3f center, float radius);
private:
	static bool isPointInTriangle(const Vector3f& point, const Vector3f triangle[]);
	static std::optional<Vector3f> raycastPlaneNoNormal(const Ray& ray, const Plane& plane);
};

