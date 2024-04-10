#include "Collisions.h"
#include <limits>
#include <span>
#include "Capsule.h"
#include "Loggy.h"

static Loggy::Logger print{ "Collisions" };


// ---- Capsule/Triangle intersection test ----
// As implemented in https://wickedengine.net/capsule-collision-detection/ by turanszkij
static Vector3f normalFromTriangle(const Vector3f triangle[]) {
	const auto& a = triangle[0];
	const auto& b = triangle[1];
	const auto& c = triangle[2];
	return Vector3f::cross(b - a, c - a).normalize();
}

typedef Vector3f float3;

float dot(float3 a, float3 b) {
	return Vector3f::dot(a, b);
}

float3 cross(float3 a, float3 b) {
	return Vector3f::cross(a, b);
}

float saturate(float v) {
	return std::min(std::max(v, 0.0f), 1.0f);
}

float3 ClosestPointOnLineSegment(float3 A, float3 B, float3 Point) {
	float3 AB = B - A;
	float t = dot(Point - A, AB) / dot(AB, AB);
	return A + AB * saturate(t); // saturate(t) can be written as: min((max(t, 0), 1)
}

bool Collisions::isIntersecting(const Capsule& caps, const Vector3f triangle[]) {
	float3 tip = caps.tip;
	float3 base = caps.base;
	float radius = caps.radius;

	float3 p0 = triangle[0];
	float3 p1 = triangle[1];
	float3 p2 = triangle[2];
	float3 N = normalFromTriangle(triangle);

	// Compute capsule line endpoints A, B like before in capsule-capsule case:
	float3 CapsuleNormal = (tip - base).normalize();
	float3 LineEndOffset = CapsuleNormal * radius;
	float3 A = base + LineEndOffset;
	float3 B = tip - LineEndOffset;

	float3 reference_point;

	// If the capsule line and triangle are parallell to each other
	bool parallel = std::abs(N.y) < 0.001;

	if (parallel) {
		reference_point = p0;
	}
	else {
		// Then for each triangle, ray-plane intersection:
		//  N is the triangle plane normal (it was computed in sphere – triangle intersection case)
		float t = dot(N, (p0 - base) / abs(dot(N, CapsuleNormal)));
		float3 line_plane_intersection = base + CapsuleNormal * t;

		// Determine whether line_plane_intersection is inside all triangle edges: 
		float3 c0 = cross(line_plane_intersection - p0, p1 - p0);
		float3 c1 = cross(line_plane_intersection - p1, p2 - p1);
		float3 c2 = cross(line_plane_intersection - p2, p0 - p2);
		bool inside = dot(c0, N) <= 0 && dot(c1, N) <= 0 && dot(c2, N) <= 0;

		if (inside)
		{
			reference_point = line_plane_intersection;
		}
		else {
			// Edge 1:
			float3 point1 = ClosestPointOnLineSegment(p0, p1, line_plane_intersection);
			float3 v1 = line_plane_intersection - point1;
			float distsq = dot(v1, v1);
			float best_dist = distsq;
			reference_point = point1;

			// Edge 2:
			float3 point2 = ClosestPointOnLineSegment(p1, p2, line_plane_intersection);
			float3 v2 = line_plane_intersection - point2;
			distsq = dot(v2, v2);
			if (distsq < best_dist)
			{
				reference_point = point2;
				best_dist = distsq;
			}

			// Edge 3:
			float3 point3 = ClosestPointOnLineSegment(p2, p0, line_plane_intersection);
			float3 v3 = line_plane_intersection - point3;
			distsq = dot(v3, v3);
			if (distsq < best_dist)
			{
				reference_point = point3;
				best_dist = distsq;
			}
		}
	}

	float3 center = ClosestPointOnLineSegment(A, B, reference_point);

	if (Collisions::triangleIntersectsSphere(triangle, center, radius)) {
		return true;
	}
	return false;
}