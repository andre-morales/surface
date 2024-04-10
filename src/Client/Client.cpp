#include "Client.h"
#include "ClientImpl.h"
#include "Session.h"
#include "Config.h"
#include "Graphics/Renderer.h"
#include "Glow/Glow.h"
#include "Glow/Window.h"
#include "Glow/Texture.h"
#include "Glow/Image.h"
#include "Glow/GLContext.h"
#include "Glass/Elements/ElementType.h"
#include "Glass/Elements/Element.h"
#include "Glass/Elements/Button.h"
#include "Glass/Layouts/StackLayout.h"
#include "Glass/Paints/ColorPaint.h"
#include "Glass/Paints/TexturePaint.h"
#include "Glass/Events/MouseOverEvent.h"
#include "Glass/Events/MouseButtonEvent.h"
#include "Glass/SheetReader.h"
#include "Glass/LookAndFeel.h"
#include "Glass/Font.h"
#include "Glass/GUI.h"
#include "Glass/Elements/Text.h"
#include "Exception.h"
#include "Pointers.h"
#include "Loggy.h"
#include "Times.h"
#include <string>
#include <algorithm>
#include <cassert>

using namespace Glow;
using namespace Glass;

static std::string VERSION_STRING = "0.0.1_2 b14";
static std::string BUILD_STRING = VERSION_STRING + " Early Dev 1";
static std::string BUILD_TEXT = "Surface v" + BUILD_STRING;
static Loggy::Logger print{"Client"};
static Client* client = nullptr;

Client::Client() : 
	impl_(new ClientImpl(*this)),
	impl(*impl_),
	config(new Config()),
	renderer(new Renderer()) {
	client = this;
}

Client::~Client() {
	session = nullptr;
	client = nullptr;
}

void Client::init() {
	print("Initializing...");
	
	createWindow();
	createGUI();
}

void Client::run() {
	Glow::Window& win = *window;

	running = true;

	print("Running.");
	
	// Initialize rendering system and make window visible
	renderer->init();

	// Trigger resize once to layout UI things
	onResize(win.getWidth(), win.getHeight());

	// Main loop
	auto lastUpdate = Time::now();
	auto lastSecondStamp = lastUpdate;
	int frameCounter = 0;
	while (running) {
		auto now = Time::now();
		timeDelta = Time::toMillis(now - lastUpdate);

		if (session) {
			session->update();
		}

		renderer->render();
		win.swapBuffers();
		Glow::processEvents();

		frameCounter++;
		if (Time::elapsedBetween(lastSecondStamp, now) >= 1) {
			lastSecondStamp = now;
			fps = frameCounter;
			frameCounter = 0;
		}
		lastUpdate = now;
	}
}

void Client::dispose() {
	session = nullptr;
	gui = nullptr;
	window->dispose();
	window = nullptr;
}

void Client::createWindow() {
	Glow::init();
	window = mkUnique<Glow::Window>();
	Glow::Window& win = *window;

	win.build();
	win.setVSync(true);

	win.addCloseRequestListener([&]() {
		print("Close request received.");

		running = false;
	});

	win.addResizeListener([&](int w, int h) {
		onResize(w, h);
	});
}

void Client::createGUI() {
	// Instantiate GUI
	gui = mkUnique<Glass::GUI>(*window);
	Glow::Window& win = *window;
	Glass::GUI& gui = *this->gui;
	
	impl.mkUIResources();

	// Instantiate Look and Feel and read prototype info
	Glass::LookAndFeel laf { gui };
	laf.readPrototypes("res/water.xml");

	// Read main gui
	auto comps = laf.readFromFile("res/main.xml");

	// Add it to the gui instance
	for (auto& comp : comps) {
		gui.getRoot()->add(std::move(comp));
	}
	
	// Show version
	((Text*)gui.getElementById("debug-text"))->text = Client::getBuildText();

	gui.find("#start-btn")->onMouseButton([&](auto ev) {
		if (ev.action != InputAction::PRESS) return;
	
		createSession();
	});

	gui.find("#quit-btn")->onMouseButton([&](auto ev) {
		if (ev.action != InputAction::PRESS) return;

		gui.find("#main-screen")->setVisible(true);
		gui.find("#pause-screen")->setVisible(false);
		session.reset();
	});
}

void Client::createSession() {
	session = mkUnique<Session>();

	gui->find("#main-screen")->setVisible(false);
	window->setMouseVisible(false);
	
	session->start();
}

void Client::onResize(int w, int h) {
	renderer->resize(w, h);
}

Client& Client::get() {
	assert(client != nullptr);
	return *client;
}

Glow::Window* Client::getWindow() const {
	return window.get();
}

Session* Client::getSession() const {
	return session.get();
}

Renderer* Client::getRenderer() const {
	return renderer.get();
}

Glass::GUI* Client::getGUI() const {
	return gui.get();
}

Config& Client::getConfig() const {
	return *config;
}

float Client::getTimeDelta() const {
	return timeDelta;
}

int Client::getFPS() const {
	return fps;
}

const char* Client::getBuildText() {
	return BUILD_TEXT.c_str();
}