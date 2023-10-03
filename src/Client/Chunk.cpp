#include "Chunk.h"
#include "Math/Maths.h"
#include "Math/noise.h"
#include "Client\MarchingCube.h"

Chunk::Chunk() {}

void Chunk::allocate() {
	blocks = (BlockData*) new int[16 * 16 * 16]();
}

void Chunk::generate() {

	(*blocks)[4][4][4] = 2;
	(*blocks)[5][4][4] = 2;
	(*blocks)[8][4][4] = 2;

	(*blocks)[8][2][4] = 2;
	(*blocks)[5][4][4] = 3;
	(*blocks)[4][4][6] = 3;
	(*blocks)[4][4][7] = 3;

	Noise terrainGen{ 0 };

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			int bx = x + (cx * 24);
			int bz = z + (cz * 24);

			double np = 32;
			double sc = 0.039196174894183;
			double pn = terrainGen.get(np + bx * sc, np + bz * sc, 0);
			double ns = (0.7 + pn) * 8;

			uint16 n = Math::clamp(ns, 0.0, 15.0);

			(*blocks)[x][n][z] = 1;
			for (int y = 1; y < n; y++) {
				(*blocks)[x][y][z] = { 1 };
			}

			//(*blocks)[x][0][z] = { 1 };
		}
	}
}

void Chunk::batch() {
	vertices.clear();
	uvs.clear();
	normals.clear();

	for (int x = 0; x <= 14; x++) {
		for (int y = 0; y <= 14; y++) {
			for (int z = 0; z <= 14; z++) {
				Marching::march(vertices, uvs, normals, blocks, x, y, z);
			}
		}
	}
}