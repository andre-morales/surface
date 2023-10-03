#pragma once
#include "Graphics/Renderer.h"
#include "Pointers.h"

namespace Glow {
	class Window;
}

namespace Glass {
	class GUI;
}

class ClientImpl {
public:
	class Client& This;

	ClientImpl(Client&);

	void mkUIResources();
};

