#include "World.h"
#include "Chunk.h"

Chunk* World::getChunk(int cx, int cz) const {
	for (Chunk* c : chunks) {
		if (c->cx == cx && c->cz == cz) {
			return c;
		}
	}

	return nullptr;
}

Block World::getBlockAt(int x, int y, int z) const {
	if (y < 0 || y > 15) return 0;

	int cx = x / 16;
	int cz = z / 16;
	if (x < 0) cx = (x + 1) / 16 - 1;
	if (z < 0) cz = (z + 1) / 16 - 1;

	int bx = x - cx * 16;
	int bz = z - cz * 16;

	auto c = getChunk(cx, cz);
	if (!c) return 0;

	return (*c->blocks)[bx][y][bz];
}

void World::setBlockAt(Block block, int x, int y, int z) {
	if (y < 0 || y > 15) return;

	//printf("Placed at x: %i, y: %i, z: %i\n", x, y, z);

	int cx = x / 16;
	int cz = z / 16;
	if (x < 0) cx = (x + 1) / 16 - 1;
	if (z < 0) cz = (z + 1) / 16 - 1;

	int bx = x - cx * 16;
	int bz = z - cz * 16;

	auto c = getChunk(cx, cz);
	if (!c) return;

	(*c->blocks)[bx][y][bz] = block;
	c->batch();

	if (bx == 0) {
		auto nc = getChunk(cx - 1, cz);
		if (nc) nc->batch();
	}
	if (bz == 0) {
		auto nc = getChunk(cx, cz - 1);
		if (nc) nc->batch();
	}
	if (bx == 0 && bz == 0) {
		auto nc = getChunk(cx - 1, cz - 1);
		if (nc) nc->batch();
	}
}
