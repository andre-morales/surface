#pragma once
#include "Math/Vec3.h"
#include "Pointers.h"
#include <vector>
#include <optional>
#include <array>

class Chunk;
class World;
class Player;

class Session {
public:
	Session();
	~Session();

	void start();
	void setupEvents();
	void update();

	void doChunkGeneration();
	void doMovement();
	void doDebugText();

	World& getWorld() const;
	Player& getPlayer() const;

private:
	friend class Player;

	unique<World> world;
	unique<Player> player;
	bool menuVisible = false;
	bool physics = true;

	Vector3f lookingWorldPos;
	std::array<Vector3f, 3> lookingWorldTriangle;

	void* winKeyListener = nullptr;
	void* winMouseMoveListener = nullptr;
	void* winMouseButtonListener = nullptr;
};

