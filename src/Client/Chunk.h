#pragma once
#include "Types.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include <vector>

class Chunk {
public:
	typedef uint8_t BlockData[16][16][16];

	int cx = 0, cz = 0;
	BlockData* blocks;
	std::vector<Vector3f> vertices;
	std::vector<Vector2f> uvs;
	std::vector<Vector3f> normals;

	Chunk();

	void allocate();
	void generate();
	void batch();
};

