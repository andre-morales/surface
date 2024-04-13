#pragma once
#include "Types.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Block.h"
#include <vector>

class Session;

class Chunk {
public:
	typedef Block BlockData[16][16][16];

	const Session& session;
	int cx = 0, cz = 0;
	BlockData* blocks = nullptr;
	bool batched = false;

	std::vector<Vector3f> vertices;
	std::vector<Vector2f> uvs;
	std::vector<Vector3f> normals;
	std::vector<Vector3f> colors;

	Chunk(const Session&);
	~Chunk();

	void allocate();
	void generate();
	void batch();
private:
	Chunk(const Chunk&) = delete;
	Chunk(Chunk&&) = delete;
};

