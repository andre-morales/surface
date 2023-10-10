#pragma once
#include <vector>
#include "Math\Vec3.h"
#include "Math\Vec2.h"

class Chunk;

class MeshGenerator {
public:
	static void generate(Chunk& chunk);

private:
	static void march(Chunk&, int x, int y, int z);
};