#pragma once
#include "Math/Vec3.h"
#include "Pointers.h"

namespace Glow {
	class GLContext;
}

class Camera;

class Renderer {
public:
	bool debugOutlines = false;

	Renderer();
	~Renderer();

	void init();
	void render();
	void resize(int width, int height);
	void renderSession();

	void drawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color);
	void _recompileShader();
	Camera& getCamera() const;
	static Renderer& get();
	
private:
	const unique<Camera> cameraP;
	Camera& camera;
	void drawDebugOutlines();
};

