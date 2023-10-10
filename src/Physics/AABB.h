#pragma once
#include "Math/Vec3.h"
#include <array>

class AABB {
public:
	Vector3f center, extents;
	//Vector3f min, max;

	AABB() {
		extents = { 0.5, 0.5, 0.5 };
	}

	AABB(const Vector3f& center, const Vector3f& extents) {
		this->center = center;
		this->extents = extents;
	}

	std::pair<Vector3f, Vector3f> getMinMax() const {
		return { center - extents, center + extents };
	}

	std::array<Vector3f, 8> vertices() const {
		auto [min, max] = getMinMax();

		return {
			Vector3f{min.x, min.y, min.z }, // 0
			Vector3f{min.x, min.y, max.z }, // 1
			Vector3f{min.x, max.y, min.z }, // 2
			Vector3f{min.x, max.y, max.z }, // 3
			Vector3f{max.x, min.y, min.z }, // 4 
			Vector3f{max.x, min.y, max.z }, // 5
			Vector3f{max.x, max.y, min.z }, // 6
			Vector3f{max.x, max.y, max.z }, // 7
		};
	}
};