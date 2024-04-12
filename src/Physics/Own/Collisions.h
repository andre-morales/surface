#pragma once
#include "Collision.h"
#include <optional>

class Ray;
class AABB;
class Plane;
class Capsule;
class Sphere;

class Collisions {
public:
	static std::optional<Vector3f> raycastTriangle(const Ray& ray, const Vector3f triangle[]);
	static std::optional<Collision> doCapsuleTriangle(const Capsule& caps, const Vector3f triangle[]);
	static std::optional<Collision> doSphereTriangle(const Sphere& sphere, const Vector3f triangle[]);
	
	static bool doAABBTriangle(const AABB& box, const Vector3f triangle[]);

private:
	static bool isPointInTriangle(const Vector3f& point, const Vector3f triangle[]);
	static std::optional<Vector3f> raycastPlaneNoNormal(const Ray& ray, const Plane& plane);
};

