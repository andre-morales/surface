#pragma once
#include "Math/Matrices.h"
#include "Math/Vecs.h"

class Camera {
public:
	Vector3f position = Vector3f(0, 0, 2);
	Vector2f rotation = Vector2f(0, 0);

	Math::Mat4f projection, rotatedView, modelView, transform;

	float aspect = 4.0 / 3.0;
	float fov = 90;
	float near = 0.1;
	float far = 1000;

	Camera() {}
	void makeProjection();
	void makeRotatedView();
	void makeModelView();
	void makeTransform();
	Vector3f getLookDirection();
private:
	Camera(const Camera&) = delete;
};