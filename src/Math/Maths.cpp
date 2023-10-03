#include "Math/Maths.h"

namespace Math {
	void planeFromTriangle(const Vector3f triangle[], Vector3f* direction, float* distance) {
		const auto& vertA = triangle[0];
		const auto& vertB = triangle[1];
		const auto& vertC = triangle[2];

		*direction = Vector3f::cross(vertB - vertA, vertC - vertA).normalize();
		*distance = Vector3f::dot(*direction, vertA);
	}
}