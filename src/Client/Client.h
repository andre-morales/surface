#pragma once
#include "Pointers.h"

namespace Glow {
	class Window;
}
namespace Glass {
	class GUI;
}
class Renderer;
class Session;
class ClientImpl;
class Config;

class Client {
public:
	Client();
	~Client();

	void init();
	void run();
	void dispose();

	Glow::Window* getWindow() const;
	Session* getSession() const;
	Renderer* getRenderer() const;
	Glass::GUI* getGUI() const;
	Config& getConfig() const;
	float getTimeDelta() const;
	int getFPS() const;

	static Client& get();
	static const char* getBuildText();
private:
	friend class ClientImpl;
	friend class Renderer;

	unique<ClientImpl> impl_;
	ClientImpl& impl;

	unique<Config> config;
	unique<Glow::Window> window;
	unique<Renderer> renderer;
	unique<Glass::GUI> gui;
	unique<Session> session;

	int fps = 0;
	float timeDelta = 0;
	
	bool running = false;

	void createWindow();
	void createGUI();
	void createSession();
	void onResize(int width, int height);
};