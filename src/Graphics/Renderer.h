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

	void drawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color);
	void _recompileShader();
	static Renderer& get();
private:
	void drawDebugOutlines();
};

