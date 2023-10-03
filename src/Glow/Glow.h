#pragma once

namespace Glow {
	class Window;
	class GLContext;

	void init();
	void processEvents();
	void terminate();

	/* Returns a pointer to the current GLContext associated with the current thread, or null if
	none is.*/
	GLContext* getContext();
	void setContextWindow(Window* window);
	Window* getContextWindow();
	Window* getHandleWindow(void* handle);
}