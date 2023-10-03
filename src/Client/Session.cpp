#include "Session.h"
#include "Client/Client.h"
#include "Glow/Window.h"
#include "Graphics/Renderer.h"
#include "Glow/Inputs.h"
#include "Glass/GUI.h"
#include "Glass/Elements/Text.h"
#include <format>
#include "Loggy.h"
#include "Math\Maths.h"
#include "Types.h"
#include "Chunk.h"
#include "Physics\Collisions.h"
#include "Physics\Ray.h"

static Loggy::Logger print{ "Session" };

#define RADS (3.14159265359/180.0)

using namespace Glow;
using namespace Glass;

Session::Session() {
	
}

Session::~Session() {
	print("Destroying...");
	Glow::Window& win = *Client::get().getWindow();
	win.removeKeyListener(winKeyListener);
	winKeyListener = nullptr;

	win.removeMouseMoveListener(winMouseMoveListener);
	winMouseMoveListener = nullptr;

	win.removeMouseListener(winMouseButtonListener);
	winMouseButtonListener = nullptr;
	print("Destroyed.");
}

std::optional<Vector3i> Session::getLookingBlock() {
	auto pos = getLookingWorldPos();
	if (!pos) return {};

	auto hit = *pos;
	hit += Vector3f(0.5f);

	Vector3i hitI{ hit };
	
	auto [x, y, z] = hitI.vec;
	if ((*chunks[0]->blocks)[x][y][z] == 0) {
		auto lookdir = Client::get().getRenderer()->camera.getLookDirection();

		float xr = abs((std::ceil(hit.x) - hit.x) / lookdir.x);
		float yr = abs((std::ceil(hit.y) - hit.y) / lookdir.y);
		float zr = abs((std::ceil(hit.z) - hit.z) / lookdir.z);

		if (xr < yr && xr < zr) {
			x += lookdir.x / abs(lookdir.x);
		}
		if (yr < xr && yr < zr) {
			y += lookdir.y / abs(lookdir.y);
		}
		if (zr < xr && zr < yr) {
			z += lookdir.z / abs(lookdir.z);
		}

	}

	return { {x, y, z} };
}

std::optional<Vector3i> Session::getLookingBlockForPlace() {
	Client& client = Client::get();
	Camera& cam = client.getRenderer()->camera;

	Vector3f hit;
	Vector3f* triangle;

	if (!cameraRaycast(&hit, &triangle)) {
		return {};
	}

	hit += Vector3f(0.5f);

	Vector3i hitI{ hit };

	auto [x, y, z] = hitI.vec;
	if ((*chunks[0]->blocks)[x][y][z] != 0) {
		Vector3f triangleDir;
		float distance;
		Math::planeFromTriangle(triangle, &triangleDir, &distance);

		auto lookDir = -Client::get().getRenderer()->camera.getLookDirection();
		
		auto dir = (triangleDir + lookDir);

		float xr = abs((std::ceil(hit.x) - hit.x) / dir.x);
		float yr = abs((std::ceil(hit.y) - hit.y) / dir.y);
		float zr = abs((std::ceil(hit.z) - hit.z) / dir.z);

		if (xr < yr && xr < zr) {
			x += dir.x / abs(dir.x);
		}
		if (yr < xr && yr < zr) {
			y += dir.y / abs(dir.y);
		}
		if (zr < xr && zr < yr) {
			z += dir.z / abs(dir.z);
		}

	}

	return { {x, y, z} };
}

bool Session::cameraRaycast(Vector3f* outHit, Vector3f** outTriangle) {
	Client& client = Client::get();
	Camera& cam = client.getRenderer()->camera;

	Chunk* chunk = chunks[0];
	auto& verts = chunk->vertices;

	float closestDistance = 1000000;
	Vector3f closestHit;
	Vector3f* closestTriangle = nullptr;

	for (int i = 0; i < chunk->vertices.size(); ) {
		auto v0 = verts[i];
		auto v1 = verts[i + 1];
		auto v2 = verts[i + 2];

		Ray ray;
		ray.position = cam.position;
		ray.direction = cam.getLookDirection();

		auto hit = Collisions::raycastTriangle(ray, &verts[i]);

		if (hit) {
			float len = (cam.position - *hit).length();
			if (len < closestDistance) {
				closestDistance = len;
				closestHit = *hit;
				closestTriangle = &verts[i];
			}
		}

		i += 3;
	}

	if (!closestTriangle) return false;

	if (outHit) *outHit = closestHit;
	if (outTriangle) *outTriangle = closestTriangle;
	return true;
}

std::optional<Vector3f> Session::getLookingWorldPos() {
	Client& client = Client::get();
	Camera& cam = client.getRenderer()->camera;

	Vector3f hit;
	if (cameraRaycast(&hit, nullptr)) {
		return hit;
	} else {
		return {};
	}
}

void Session::start() {
	print("Starting...");
	Client& client = Client::get();
	Window& window = *client.getWindow();
	Renderer& render = *client.getRenderer();
	Camera& cam = client.getRenderer()->camera;
	GUI& gui = *client.getGUI();

	Glow::Window& win = *client.getWindow();

	winKeyListener = win.addKeyListener([&](int key, int scan, int action, int mods) {
		if (action != (int)InputAction::PRESS) return;

		if (key == (int)InputKey::ESC) {
			menuVisible = !menuVisible;
			gui.getElementById("pause-screen")->setVisible(menuVisible);
			window.setMouseVisible(menuVisible);
		}
	});

	winMouseMoveListener = win.addMouseMoveListener([this, &cam](double mx, double my) {
		if (lastMX != -1 && !menuVisible) {
			auto& rotation = cam.rotation;
			rotation.x += (my - lastMY) * 1;
			rotation.y += (mx - lastMX) * 1;

			if (rotation.x < -90) rotation.x = -90;
			if (rotation.x > 90) rotation.x = 90;

			if (rotation.y > 180) rotation.y -= 360;
			if (rotation.y < -180) rotation.y += 360;
		}
		lastMX = mx;
		lastMY = my;
	});

	winMouseButtonListener = win.addMouseListener([&](MouseButton btn, InputAction action, int modifiers) {
		if (action == InputAction::PRESS) {
			if (chunks.size() == 0) return;

			Chunk* chunk = chunks[0];

			if (btn == MouseButton::LEFT) {
				auto hitOpt = getLookingBlock();
				if (!hitOpt) return;

				auto hit = *hitOpt;
				auto [x, y, z] = hit.vec;

				(*chunk->blocks)[x][y][z] = 0;
				chunk->batch();
				
			} else {
				auto hitOpt = getLookingBlockForPlace();
				if (!hitOpt) return;

				auto hit = *hitOpt;
				auto [x, y, z] = hit.vec;

				(*chunk->blocks)[x][y][z] = 1;
				chunk->batch();
				
			}
		}
	});

	print("Ready.");
}

void Session::update() {
	Client& client = Client::get();
	Glow::Window& win = *client.getWindow();
	Camera& cam = client.getRenderer()->camera;
	
	float timeDelta = client.getTimeDelta();
	
	for (int x = 0; x <= 0; x++) {
		for (int z = 0; z <= 0; z++) {
			int cx = x + (int)(cam.position.x / 16);
			int cz = z + (int)(cam.position.z / 16);

			bool found = false;
			for (auto& c : chunks) {
				if (c->cx == cx && c->cz == cz) {
					found = true;
					break;
				}
			}

			if (!found) {
				auto chunk = new Chunk();
				chunk->cx = cx;
				chunk->cz = cz;
				chunk->allocate();
				chunk->generate();
				chunk->batch();
				chunks.emplace_back(chunk);
			}
		}
	}

	float movementSpeed = 5;
	
	if (win.getKey(InputKey::W)) {
		cam.position.x += timeDelta * movementSpeed * std::sin(cam.rotation.y * RADS);
		cam.position.z -= timeDelta * movementSpeed * std::cos(cam.rotation.y * RADS);
	} else if (win.getKey(InputKey::S)) {
		cam.position.x -= timeDelta * movementSpeed * std::sin(cam.rotation.y * RADS);
		cam.position.z += timeDelta * movementSpeed * std::cos(cam.rotation.y * RADS);
	}

	if (win.getKey(InputKey::A)) {
		cam.position.x -= timeDelta * movementSpeed * std::cos(cam.rotation.y * RADS);
		cam.position.z -= timeDelta * movementSpeed * std::sin(cam.rotation.y * RADS);
	}
	if (win.getKey(InputKey::D)) {
		cam.position.x += timeDelta * movementSpeed * std::cos(cam.rotation.y * RADS);
		cam.position.z += timeDelta * movementSpeed * std::sin(cam.rotation.y * RADS);
	}

	if (win.getKey(InputKey::SPACE)) {
		cam.position.y += timeDelta * movementSpeed;
	}
	if (win.getKey(InputKey::L_SHIFT)) {
		cam.position.y -= timeDelta * movementSpeed;
	}

	Glass::GUI& gui = *client.getGUI();
	
	std::string debugText = std::format(R"(
MyGame v0.0.1:
FPS: {}
X: {:.2f} Y: {:.2f} Z: {:.2f}
RX: {:.2f}, RY: {:.2f})",
client.fps,
cam.position.x, cam.position.y, cam.position.z,
cam.rotation.x, cam.rotation.y);

	((Text*)gui.getElementById("debug-text"))->text = debugText;
}











