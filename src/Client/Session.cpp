#include "Session.h"
#include "Client.h"
#include "Chunk.h"
#include "World.h"
#include "Player.h"
#include "Graphics/Renderer.h"
#include "Glow/Window.h"
#include "Glow/Inputs.h"
#include "Glass/GUI.h"
#include "Glass/Elements/Text.h"
#include "Math\Maths.h"
#include "Loggy.h"
#include "Types.h"
#include "Physics/Engine.h"
#include <format>

static Loggy::Logger print{ "Session" };

#define RADS (3.14159265359/180.0)

int selectedBlock = 1;

using namespace Glow;
using namespace Glass;

Session::Session() {}

Session::~Session() {
	print("Destroying...");

	// Remove all of our listeners from Window to prevent zombie listeners
	auto& win = *Client::get().getWindow();
	win.removeKeyListener(winKeyListener);
	winKeyListener = nullptr;

	win.removeMouseMoveListener(winMouseMoveListener);
	winMouseMoveListener = nullptr;

	win.removeMouseListener(winMouseButtonListener);
	winMouseButtonListener = nullptr;
	
	print("Destroyed.");
}

void Session::start() {
	print("Starting...");
	
	player = mkUnique<Player>(*this);
	world = mkUnique<World>();

	setupListeners();

	Physics::initWorld();
	Physics::enableDebugging();
	Physics::initBulletWorldPhysics();

	print("Ready.");
}

void Session::setupListeners() {
	Client& client = Client::get();
	Window& window = *client.getWindow();
	Player& player = getPlayer();
	Renderer& render = *client.getRenderer();
	Camera& cam = render.camera;
	GUI& gui = *client.getGUI();

	winKeyListener = window.addKeyListener([&](InputKey key, int scan, InputAction action, int mods) {
		if (action != InputAction::PRESS) return;

		if (key >= InputKey::NUM_1 && key <= InputKey::NUM_9) {
			selectedBlock = 1 + ((int)key - (int)InputKey::NUM_1);
		}

		if (key == InputKey::F2) {
			render._recompileShader();
		}

		if (key == InputKey::F4) {
			render.debugOutlines = !render.debugOutlines;
		}

		if (key == InputKey::F5) {
			noClipping = !noClipping;
		}

		if (key == InputKey::ESC) {
			menuVisible = !menuVisible;
			gui.getElementById("pause-screen")->setVisible(menuVisible);
			window.setMouseVisible(menuVisible);
		}
		});

	float lastMX = -1, lastMY = -1;
	winMouseMoveListener = window.addMouseMoveListener([&, lastMX, lastMY](float mx, float my) mutable {
		if (lastMX != -1 && !menuVisible) {
			auto& rot = player.rotation;
			rot.x += (my - lastMY) * 1;
			rot.y += (mx - lastMX) * 1;

			if (rot.x < -90) rot.x = -90;
			if (rot.x > 90) rot.x = 90;

			if (rot.y > 180) rot.y -= 360;
			if (rot.y < -180) rot.y += 360;
		}

		lastMX = mx;
		lastMY = my;
		});

	winMouseButtonListener = window.addMouseListener([&](MouseButton btn, InputAction action, int modifiers) {
		if (action == InputAction::PRESS) {
			if (world->chunks.size() == 0) return;

			Chunk* chunk = world->chunks[0];

			if (btn == MouseButton::LEFT) {
				auto hitOpt = cam.getLookingBlockPos();
				if (!hitOpt) return;

				auto hit = *hitOpt;
				auto [x, y, z] = hit.vec;

				world->setBlockAt(0, x, y, z);

			}
			else {
				auto hitOpt = cam.getLookingEmptyBlockPos();
				if (!hitOpt) return;

				auto hit = *hitOpt;
				auto [x, y, z] = hit.vec;

				world->setBlockAt(selectedBlock, x, y, z);
			}
		}
		});
}

void Session::update() {
	Client& client = Client::get();
	Glow::Window& win = *client.getWindow();
	Camera& cam = client.getRenderer()->camera;
	Glass::GUI& gui = *client.getGUI();

	doMovement();
	doChunkGeneration();
	cam.doCameraRaycast();
	doDebugText();
}

void Session::doChunkGeneration() {
	world->doLoadChunks();
}

void Session::doMovement() {
	Client& client = Client::get();
	Player& player = getPlayer();
	Window& win = *client.getWindow();
	Camera& cam = client.getRenderer()->camera;

	float timeDelta = client.getTimeDelta();
	float movementSpeed = 5;

	Vector3f mVel{};

	if (win.getKey(InputKey::W)) {
		mVel.x += movementSpeed * std::sin(cam.rotation.y * RADS);
		mVel.z -= movementSpeed * std::cos(cam.rotation.y * RADS);
	} else if (win.getKey(InputKey::S)) {
		mVel.x -= movementSpeed * std::sin(cam.rotation.y * RADS);
		mVel.z += movementSpeed * std::cos(cam.rotation.y * RADS);
	}

	if (win.getKey(InputKey::A)) {
		mVel.x -= movementSpeed * std::cos(cam.rotation.y * RADS);
		mVel.z -= movementSpeed * std::sin(cam.rotation.y * RADS);
	} else if (win.getKey(InputKey::D)) {
		mVel.x += movementSpeed * std::cos(cam.rotation.y * RADS);
		mVel.z += movementSpeed * std::sin(cam.rotation.y * RADS);
	}

	if (win.getKey(InputKey::SPACE)) {
		mVel.y += movementSpeed;
	} else if (win.getKey(InputKey::L_SHIFT)) {
		mVel.y -= movementSpeed;
	}

	player.motion = mVel;
	player.velocity = player.velocity * 0.985f + mVel * 0.025f;

	Physics::simulate(timeDelta);
	player.doPhysics(timeDelta);
	//player.doWorldPhysics();

	cam.position = player.position;
	cam.position.y += 1.65 + 0.5 * 2;
	cam.position.y += 0;
	cam.rotation = player.rotation;
}

void Session::doDebugText() {
	Client& client = Client::get();
	Camera& cam = client.getRenderer()->camera;
	Player& ply = client.getSession()->getPlayer();
	Glass::GUI& gui = *client.getGUI();

	std::string debugText = std::format(R"({}
FPS: {}
Camr X: {:.2f} Y: {:.2f} Z: {:.2f}
Plyr X: {:.2f} Y: {:.2f} Z: {:.2f}
RX: {:.2f}, RY: {:.2f})",
Client::getBuildText(),
client.getFPS(),
cam.position.x, cam.position.y, cam.position.z,
ply.position.x, ply.position.y, ply.position.z,
cam.rotation.x, cam.rotation.y);

	((Text*)gui.getElementById("debug-text"))->text = debugText;
}

World& Session::getWorld() const {
	return *world;
}

Player& Session::getPlayer() const {
	return *player;
}