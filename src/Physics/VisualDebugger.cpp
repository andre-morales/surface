#include "VisualDebugger.h"
#include "Graphics/Renderer.h"

Vector3f convert(const btVector3 v) {
	return { v.x(), v.y(), v.z() };
}

void VisualDebugger::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	Renderer::get().drawLine(convert(from), convert(to), convert(color));
}

void VisualDebugger::setDebugMode(int mode) {
	this->debugMode = mode;
}

int VisualDebugger::getDebugMode() const {
	return debugMode;
}

void VisualDebugger::reportErrorWarning(const char* msg) {
	printf("[BULLET] %s\n", msg);
}

void VisualDebugger::drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) {}

void VisualDebugger::draw3dText(const btVector3&, const char*) {}
