#include "Collisions.h"
#include <limits>
#include <span>

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





Vector3f normalFromTriangle(const Vector3f triangle[]) {
	const auto& a = triangle[0];
	const auto& b = triangle[1];
	const auto& c = triangle[2];
	return Vector3f::cross(b - a, c - a).normalize();
}

void Project(std::span<const Vector3f> points, Vector3f axis, double* min, double* max);

bool Collisions::IsIntersecting(AABB& box, const Vector3f triangle[]) {
	auto [boxMinV, boxMaxV] = box.getMinMax();
	
	std::span<const Vector3f> triangleSpan{ triangle, 3 };
    double triangleMin, triangleMax;
    double boxMin, boxMax;

    // Test the box normals (x-, y- and z-axes)
	Vector3f boxNormals[]{
		Vector3f(1,0,0),
		Vector3f(0,1,0),
		Vector3f(0,0,1)
	};

    for (int i = 0; i < 3; i++) {
      //  auto n = boxNormals[i];
		Project(triangleSpan, boxNormals[i], &triangleMin, &triangleMax);

        if (triangleMax < boxMinV.vec[i] || triangleMin > boxMaxV.vec[i])
            return false; // No intersection possible.
    }

    // Test the triangle normal
	
	//double triangleOffset = triangle.Normal.Dot(triangle.A);
	Vector3f triangleNormal = normalFromTriangle(triangle);
	double triangleOffset = Vector3f::dot(triangleNormal, triangle[0]);
    
    //Project(box.Vertices, triangle.Normal, out boxMin, out boxMax);
	Project(box.vertices(), triangleNormal, &boxMin, &boxMax);
    if (boxMax < triangleOffset || boxMin > triangleOffset)
        return false; // No intersection possible.

    // Test the nine edge cross-products
    Vector3f triangleEdges[] {
        triangle[0] - triangle[1],
        triangle[1] - triangle[2],
        triangle[2] - triangle[0]
    };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            // The box normals are the same as it's edge tangents
            Vector3f axis = Vector3f::cross(triangleEdges[i], boxNormals[j]);
            Project(box.vertices(), axis, &boxMin, &boxMax);
            Project(triangleSpan, axis, &triangleMin, &triangleMax);
            if (boxMax < triangleMin || boxMin > triangleMax)
                return false; // No intersection possible
        }

    // No separating axis found.
    return true;
}

void Project(std::span<const Vector3f> points, Vector3f axis, double* min, double* max)
{
    *min = std::numeric_limits<double>::max();
    *max = std::numeric_limits<double>::lowest();

    for (auto& p : points)  {
        double val = Vector3f::dot(axis, p);
        if (val < *min) *min = val;
        if (val > *max) *max = val;
    }
}