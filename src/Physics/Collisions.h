#pragma once
#include "Ray.h"
#include "Plane.h"
#include <optional>

class Collisions {
public:
	typedef std::span<const Vector3f> TriangleView;

	static std::optional<Vector3f> raycastTriangle(const Ray& ray, const Vector3f triangle[]);

private:
	static bool pointInTriangle(const Vector3f& point, const Vector3f triangle[]);

	static std::optional<Vector3f> raycastPlaneNoNormal(const Ray& ray, const Plane& plane);

	//static bool raycastPlane(Ray ray, Plane plane, Math::Vec3f* hitPoint);
};

