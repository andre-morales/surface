#include "Glow.h"
#include "Window.h"
#include "Loggy.h"
#include "Exception.h"
#include "GLFW/glfw3.h"

using namespace std::string_literals;

static Loggy::Logger print{"Glow"};

namespace Glow {
	thread_local Window* currentContextWindow = nullptr;

	void init() {
		print("Initializing GLFW...");
		if (glfwInit() != GLFW_TRUE) {
			throw Exception("GLFW Initialization failed.");
		}
		glfwSetErrorCallback([](int error, const char* text) {
			print("ERROR ", error, " : ", text);
		});
		print("Ok.");
	}

	void processEvents() {
		glfwPollEvents();
	}

	void terminate() {
		print("Terminating...");
		glfwTerminate();
		printf("Terminated.");
	}

	void setContextWindow(Window* win) {
		currentContextWindow = win;
		auto* nwin = (GLFWwindow*)win->getNativeHandle();
		glfwMakeContextCurrent(nwin);
	}

	Window* getContextWindow() {
		return currentContextWindow;
	}

	GLContext* getContext() {
		return currentContextWindow->getContext();
	}

	Window* getHandleWindow(void* gw) {
		GLFWwindow* handle = (GLFWwindow*)gw;

		return (Window*)glfwGetWindowUserPointer(handle);
	}
}