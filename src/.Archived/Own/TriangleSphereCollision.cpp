#include "Collisions.h"
#include <limits>
#include <span>
#include "Sphere.h"
#include "Loggy.h"

static float saturate(float v) {
	return std::min(std::max(v, 0.0f), 1.0f);
}

static Vector3f ClosestPointOnLineSegment(Vector3f A, Vector3f B, Vector3f Point) {
	Vector3f AB = B - A;
	float t = Vector3f::dot(Point - A, AB) / AB.lengthSquared();
	return A + AB * saturate(t);
}

std::optional<Collision> Collisions::doSphereTriangle(const Sphere& sphere, const Vector3f triangle[]) {
	auto& center = sphere.center;
	float radius = sphere.radius;

	auto& p0 = triangle[0];
	auto& p1 = triangle[1];
	auto& p2 = triangle[2];

	Vector3f N = (Vector3f::cross(p1 - p0, p2 - p0)).normalize(); // plane normal
	float dist = Vector3f::dot(center - p0, N); // signed distance between sphere and plane
	
	//if (!mesh.is_double_sided() && dist > 0)
	//	continue; // can pass through back side of triangle (optional)
	if (dist <= 0) return {};

	if (dist < -radius || dist > radius)
		return {};

	Vector3f point0 = center - N * dist; // projected sphere center on triangle plane

	// Now determine whether point0 is inside all triangle edges: 
	Vector3f c0 = Vector3f::cross(point0 - p0, p1 - p0);
	Vector3f c1 = Vector3f::cross(point0 - p1, p2 - p1);
	Vector3f c2 = Vector3f::cross(point0 - p2, p0 - p2);
	bool inside = Vector3f::dot(c0, N) <= 0 && Vector3f::dot(c1, N) <= 0 && Vector3f::dot(c2, N) <= 0;

	float radiussq = radius * radius; // sphere radius squared

	// Edge 1:
	Vector3f point1 = ClosestPointOnLineSegment(p0, p1, center);
	Vector3f v1 = center - point1;
	float distsq1 = v1.lengthSquared();
	bool intersects = distsq1 < radiussq;

	// Edge 2:
	Vector3f point2 = ClosestPointOnLineSegment(p1, p2, center);
	Vector3f v2 = center - point2;
	float distsq2 = v2.lengthSquared();
	intersects |= distsq2 < radiussq;

	// Edge 3:
	Vector3f point3 = ClosestPointOnLineSegment(p2, p0, center);
	Vector3f v3 = center - point3;
	float distsq3 = v3.lengthSquared();
	intersects |= distsq3 < radiussq;

	//At the end of this, we will know whether we intersect with the triangle or not.If we intersect, we must compute the penetration depth and normal so that we will be able to properly handle the collision(bounce back or slide on surface).Like I said, we need to determine the closest intersection point for this.It is either inside the triangle, or on one of the edges :
	if (inside || intersects)
	{
		Vector3f best_point = point0;
		Vector3f intersection_vec;

		if (inside)
		{
			intersection_vec = center - point0;
		}
		else
		{
			Vector3f d = center - point1;
			float best_distsq = d.lengthSquared();
			best_point = point1;
			intersection_vec = d;

			d = center - point2;
			float distsq = d.lengthSquared();
			if (distsq < best_distsq)
			{
				distsq = best_distsq;
				best_point = point2;
				intersection_vec = d;
			}

			d = center - point3;
			distsq = d.lengthSquared();
			if (distsq < best_distsq)
			{
				distsq = best_distsq;
				best_point = point3;
				intersection_vec = d;
			}
		}

		float len = intersection_vec.length();  // vector3 length calculation: 
		Vector3f penetration_normal = intersection_vec / len;  // normalize
		float penetration_depth = radius - len; // radius = sphere radius

		Collision c{ penetration_depth, penetration_normal };
		return c; // intersection success
	}
	return {};
}
