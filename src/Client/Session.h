#pragma once
#include <vector>
#include <optional>
#include "Math/Vec3.h"

class Chunk;

class Session {
public:
	typedef int BlockData[16][16][16];

	std::vector<Chunk*> chunks;

	Session();
	~Session();

	void start();
	void update();

	bool cameraRaycast(Vector3f* hit, Vector3f** triangle);
	std::optional<Vector3i> getLookingBlock();
	std::optional<Vector3i> getLookingBlockForPlace();
	std::optional<Vector3f> getLookingWorldPos();

private:
	float lastMX = -1, lastMY = -1;
	bool menuVisible = false;
	void* winKeyListener = nullptr;
	void* winMouseMoveListener = nullptr;
	void* winMouseButtonListener = nullptr;
};

