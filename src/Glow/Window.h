#pragma once
#include "Inputs.h"
#include "Pointers.h"
#include "Math/vecs.h"
#include <functional>

namespace Glow {
	class GLContext;

	class Window {
	public:
		typedef std::function<void()> CloseReqListener;
		typedef std::function<void(int width, int height)> ResizeListener;
		typedef std::function<void(MouseButton btn, InputAction action, int modifiers)> MouseListener;
		typedef std::function<void(float mx, float my)> MouseMoveListener;
		typedef std::function<void(InputKey key, int scanCode, InputAction action, int mods)> KeyListener;

		Window();
		~Window();

		void build();
		void swapBuffers();
		void dispose();
		void setVSync(bool);

		GLContext* getContext() const;
		int getWidth() const;
		int getHeight() const;
		float getAspect() const;
		Vector2f getMousePos() const;
		void setMouseVisible(bool visible);
		void* getNativeHandle() const;
		bool getKey(InputKey key) const;

		ResizeListener* addResizeListener(ResizeListener listener);
		void removeResizeListener(void* ptr);

		CloseReqListener* addCloseRequestListener(CloseReqListener listener);
		void removeCloseRequestListener(void* ptr);

		MouseListener* addMouseListener(MouseListener listener);
		void removeMouseListener(void* ptr);

		MouseMoveListener* addMouseMoveListener(MouseMoveListener listener);
		void removeMouseMoveListener(void* ptr);

		KeyListener* addKeyListener(KeyListener listener);
		void removeKeyListener(void* ptr);

	private:
		void* handle = nullptr;
		unique<GLContext> context;
		int width = -1, height = -1;
		float aspect = -1;

		bool pressedButtons[8]{};

		std::vector<std::pair<ResizeListener*, bool>> resizeListeners;
		std::vector<std::pair<CloseReqListener*, bool>> closeReqListeners;
		std::vector<std::pair<MouseListener*, bool>> mouseListeners;
		std::vector<std::pair<MouseMoveListener*, bool>> mouseMoveListeners;
		std::vector<std::pair<KeyListener*, bool>> keyListeners;

		void setupEvents();
	};
}