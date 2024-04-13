#pragma once
#include <bullet/LinearMath/btIDebugDraw.h>

class VisualDebugger : public btIDebugDraw {
public:
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void setDebugMode(int mode) override;
	int getDebugMode() const override;
	void reportErrorWarning(const char* msg) override;
	void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override;
	void draw3dText(const btVector3&, const char*) override;

private:
	int debugMode;
};