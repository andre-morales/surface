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
	float getTimeDelta() const;

	static Client& get();
	int fps = 0;
private:
	friend class ClientImpl;
	friend class Renderer;

	unique<ClientImpl> impl_;
	ClientImpl& impl;

	unique<Renderer> renderer;
	unique<Session> session;
	unique<Glow::Window> window;
	unique<Glass::GUI> gui;

	float timeDelta = 0;
	
	bool running = false;

	void createWindow();
	void createGUI();
	void createSession();
	void onResize(int width, int height);
};