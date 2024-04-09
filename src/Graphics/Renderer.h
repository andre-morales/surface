#pragma once
#include "Graphics/Camera.h"

namespace Glow {
	class GLContext;
}

class Renderer {
public:
	Camera camera;
	bool debugOutlines = false;

	Renderer();

	void init();
	void render();
	void resize(int width, int height);
	void renderSession();

	void _recompileShader();

private:
	void drawDebugOutlines();
};

