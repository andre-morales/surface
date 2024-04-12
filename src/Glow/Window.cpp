#include "Window.h"
#include "Glow.h"
#include "GLContext.h"
#include "Loggy.h"
#include "Exception.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Debug.h"
#include <assert.h>

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

using namespace std::string_literals;

static Loggy::Logger print{"Window"};

namespace Glow {
	Window::Window() {
		width = 800;
		height = 600;
		aspect = 800 / 600.0f;
	}

	Window::~Window() {
		if (!handle) return;

		dispose();
	}

	void Window::build() {
		print("Building...");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		GLFWwindow* win = glfwCreateWindow(width, height, "Window", nullptr, nullptr);
		
		if (!win) {
			throw Exception("Window creation failed.");
		}
		
		handle = win;

		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			print("Debug mode successfully enabled.");
		}

		
		glfwSetWindowUserPointer(win, this);
		aspect = width / (float) height;

		setupEvents();
		context = mkUnique<GLContext>(*this);
		Glow::setContextWindow(this);

		print("Initializing GLEW...");
		GLenum glewStatus = glewInit();
		if (glewStatus != GLEW_OK) {
			const char* str = (const char*)glewGetErrorString(glewStatus);
			throw Exception("GLEW Initialization failed: "s + str);
		}
		print("Done.");

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glfwSwapInterval(0);
	}

	template <class ListenerType, class... Args>
	void fireAllListeners(std::vector<std::pair<ListenerType, bool>>& listeners, Args&&... args) {
		bool inactiveListeners = false;

		for (const auto& listenerPair : listeners) {

			// If listener is active
			if (listenerPair.second) {
				(*listenerPair.first)(args...);
			} else {
				inactiveListeners = true;
			}
		}

		std::erase_if(listeners, [](const auto& pair) {
			return !pair.second;
		});
	}

	void Window::swapBuffers() {
		auto gwin = (GLFWwindow*)handle;
		glfwSwapBuffers(gwin);
	}

	void Window::setVSync(bool vsync) {
		if (vsync) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}
	}

	void Window::setupEvents() {
		auto gWin = (GLFWwindow*) handle;

		glfwSetWindowCloseCallback(gWin, [](auto* nWin) {
			Window& win = *getHandleWindow(nWin);
			for (const auto& l : win.closeReqListeners) {
				(*l.first)();
			}
		});
		
		glfwSetFramebufferSizeCallback(gWin, [](auto* nWin, int w, int h) {
			Window& win = *getHandleWindow(nWin);
			win.width = w;
			win.height = h;
			win.aspect = w / (float)h;
			for (const auto& l : win.resizeListeners) {
				(*l.first)(w, h);
			}
		});
		
		glfwSetMouseButtonCallback(gWin, [](auto* nWin, int btn, int action, int mods) {
			Window& win = *getHandleWindow(nWin);

			if (action == GLFW_RELEASE) {
				win.pressedButtons[btn] = false;
			} else {
				win.pressedButtons[btn] = true;
			}

			fireAllListeners(win.mouseListeners, (MouseButton)btn, (InputAction)action, mods);
		});

		glfwSetCursorPosCallback(gWin, [](auto* nWin, double x, double y) {
			Window& win = *getHandleWindow(nWin);
			for (const auto& l : win.mouseMoveListeners) {
				(*l.first)((float)x, (float)y);
			}
		});

		glfwSetKeyCallback(gWin, [](auto* nWin, int key, int scanCode, int action, int mods) {
			Window& win = *getHandleWindow(nWin);
			for (const auto& l : win.keyListeners) {
				(*l.first)((InputKey)key, scanCode, (InputAction)action, mods);
			}
		});
	}

	void Window::dispose() {
		context = nullptr;

		auto win = (GLFWwindow*)handle;
		glfwDestroyWindow(win);
		handle = nullptr;
	}

	void* Window::getNativeHandle() const {
		return handle;
	}

	GLContext* Window::getContext() const {
		return context.get();
	}

	int Window::getWidth() const {
		int width;
		glfwGetFramebufferSize((GLFWwindow*)handle, &width, NULL);
		return width;
	}

	int Window::getHeight() const {
		int height;
		glfwGetFramebufferSize((GLFWwindow*)handle, NULL, &height);
		return height;
	}

	float Window::getAspect() const {
		return aspect;
	}

	Vector2f Window::getMousePos() const {
		double x, y;
		glfwGetCursorPos((GLFWwindow*)handle, &x, &y);

		return { (float)x, (float)y };
	}

	void Window::setMouseVisible(bool visible) {
		if (visible) {
			glfwSetInputMode((GLFWwindow*)handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		} else {
			glfwSetInputMode((GLFWwindow*)handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	bool Window::getKey(InputKey key) const {
		return glfwGetKey((GLFWwindow*)handle, (int)key) == GLFW_PRESS;
	}

	// ---- Events handling ----

	template <class T>
	void vecErase(std::vector<std::pair<T*, bool>>& vec, void* value) {
		std::pair<T*, bool> listenerPair{ (T*)value, true };

		auto it = std::find(vec.begin(), vec.end(), listenerPair);
		assert(it != vec.end());
		vec.erase(it);
	}

	template <class T>
	void vecMarkErase(std::vector<std::pair<T*, bool>>& vec, void* value) {
		std::pair<T*, bool> listenerPair{ (T*)value, true };

		auto it = std::find(vec.begin(), vec.end(), listenerPair);
		assert(it != vec.end());
		it->second = false;
	}

	template <class T>
	T* vecAdd(std::vector< std::pair<T*, bool>>& vec, T&& value) {
		T* ptr = new T(std::move(value));
		vec.emplace_back(ptr, true);
		return ptr;
	}

	Window::ResizeListener* Window::addResizeListener(ResizeListener l) {
		return vecAdd(resizeListeners, std::move(l));
	}

	void Window::removeResizeListener(void* ptr) {
		vecErase(resizeListeners, ptr);
	}

	Window::CloseReqListener* Window::addCloseRequestListener(CloseReqListener l) {
		return vecAdd(closeReqListeners, std::move(l));
	}

	void Window::removeCloseRequestListener(void* ptr) {
		vecErase(closeReqListeners, ptr);
	}

	Window::MouseListener* Window::addMouseListener(MouseListener ml) {
		return vecAdd(mouseListeners, std::move(ml));
	}

	void Window::removeMouseListener(void* ml) {
		vecMarkErase(mouseListeners, ml);
		//vecErase(mouseListeners, ml);
	}

	Window::MouseMoveListener* Window::addMouseMoveListener(MouseMoveListener ml) {
		return vecAdd(mouseMoveListeners, std::move(ml));
	}

	void Window::removeMouseMoveListener(void* ml) {
		vecErase(mouseMoveListeners, ml);
	}

	Window::KeyListener* Window::addKeyListener(KeyListener ml) {
		return vecAdd(keyListeners, std::move(ml));
	}

	void Window::removeKeyListener(void* ml) {
		vecErase(keyListeners, ml);
	}
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
	// ignore non-significant error/warning codes
	if (id == 13) return; // Ignore deprecated GL_LIGHTING message
	if (id == 7) return; // Ignore deprecated GL_LINE_WIDTH
	if (id == 131204) return; // Ignore "The texture object (0) bound to texture image unit 0 does not have a defined base level and cannot be used for texture mapping"
	
	std::cout << "-------# OPENGL DEBUG #--------" << std::endl;
	std::cout << "ID: (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;

	Debug::pause();
}
