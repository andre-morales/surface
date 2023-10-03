#pragma once
#include <vector>
#include "Math\Vec3.h"
#include "Math\Vec2.h"

class Marching {
public:
	typedef uint8_t BlockData[16][16][16];

	static void march(std::vector<Vector3f>& v, std::vector<Vector2f>& uvs, std::vector<Vector3f>& normals, BlockData* data, int x, int y, int z);
};