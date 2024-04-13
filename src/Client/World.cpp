#include "World.h"
#include "Chunk.h"
#include "Client.h"
#include "Session.h"
#include "Player.h"
#include "Config.h"

void World::doLoadChunks() {
	Client& client = Client::get();
	Session& session = *client.getSession();
	int rd = client.getConfig().renderDistance;
	auto& position = session.getPlayer().position;

	std::vector<Chunk*> unloads;
	for (Chunk* ch : chunks) {
		float dist = std::hypot(ch->cx * 16 - position.x, ch->cz * 16 - position.z);
		if (dist / 16 > rd) {
			unloads.emplace_back(ch);
		}
	}

	for (Chunk* ch : unloads) {
		removeChunk(ch);
	}

	for (int x = -rd; x <= rd; x++) {
		for (int z = -rd; z <= rd; z++) {
			int cx = x + (int)(position.x / 16);
			int cz = z + (int)(position.z / 16);
			float dist = std::hypot(cx * 16 - position.x, cz * 16 - position.z);
			if (dist / 16 > rd) continue;

			Chunk* c = getChunk(cx, cz);
			if (c) {
				if (!c->batched) c->batch();
				//c->batch();
			}
			else {
				auto chunk = new Chunk(session);
				chunk->cx = cx;
				chunk->cz = cz;
				chunk->allocate();
				chunk->generate();
				chunks.emplace_back(chunk);
			}

		}
	}
}

void World::removeChunk(Chunk* ch) {
	delete ch;
	chunks.erase(std::remove(chunks.begin(), chunks.end(), ch), chunks.end());
}

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
