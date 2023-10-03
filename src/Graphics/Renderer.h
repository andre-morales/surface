#pragma once
#include "Graphics/Camera.h"

class Renderer {
public:
	Camera camera;

	Renderer();

	void init();
	void render();
	void resize(int width, int height);
	void renderSession();
};

