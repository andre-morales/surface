#include "Chunk.h"
#include "Player.h"
#include "Client.h"
#include "Config.h"
#include "Math/Maths.h"
#include "Math/noise.h"
#include "Graphics/MeshGenerator.h"
#include "Session.h"
#include "Physics/Engine.h"
#include "World.h"

Chunk::Chunk(const Session& s) : session(s) {}

Chunk::~Chunk() {
	Physics::destroyChunkCollider(this);
}

void Chunk::allocate() {
	blocks = (BlockData*) new uint8_t[16 * 16 * 16]();
}

void Chunk::generate() {
	Noise terrainGen{ 0 };

	srand(0);

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			int bx = x + (cx * 16);
			int bz = z + (cz * 16);

			double np = 32;
			double sc = 0.039196174894183;
			double pn = terrainGen.get(np + bx * sc, np + bz * sc, 0);
			
			
			double ns = (0.7 + pn) * 10;
			uint16 n = Math::clamp(ns, 0.0, 15.0);

			(*blocks)[x][n][z] = 1;
			for (int y = 1; y < n; y++) {
				(*blocks)[x][y][z] = { 3 };
			}

			double np_ = 128;
			double sc_ = 39.938471648513;
			double pn_ = terrainGen.get(np + bx * sc, np + bz * sc, 0);
			if (rand() % 512 == 0) {
				for (int y = n + 1; y < 15; y++) {
					(*blocks)[x][y][z] = { 3 };
				}
			}
		}
	}

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			(*blocks)[x][0][z] = { 4 };
		}
	}
}

void Chunk::batch() {
	World& world = session.getWorld();

	// Neighbours
	Chunk* rightFront = world.getChunk(cx + 1, cz + 1);
	if (!rightFront) return;

	Chunk* right = world.getChunk(cx + 1, cz);
	if (!right) return;

	Chunk* front = world.getChunk(cx, cz + 1);
	if (!front) return;

	vertices.clear();
	uvs.clear();
	normals.clear();
	colors.clear();

	vertices.reserve(5000);
	uvs.reserve(5000);
	normals.reserve(5000);
	colors.reserve(5000);

	MeshGenerator::generate(*this);
	batched = true;

	Physics::createChunkCollider(this);

	//printf("-- V: %lli, U: %lli, N: %lli, C: %lli\n", vertices.size(), uvs.size(), normals.size(), colors.size());
	//printf("@@ V: %lli, U: %lli, N: %lli, C: %lli\n", vertices.capacity(), uvs.capacity(), normals.capacity(), colors.capacity());
}
