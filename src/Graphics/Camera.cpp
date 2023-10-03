#include "Camera.h"
#include "Client/Client.h"
#include "Glow/Window.h"
#define RADS (3.14159265359/180.0)

void Camera::makeProjection() {
	projection = Math::Mat4f::perspective(aspect, fov, near, far);
}

void Camera::makeRotatedView() {
	rotatedView = Math::Mat4f::rotationY(rotation.y * RADS) * Math::Mat4f::rotationX(rotation.x * RADS);
}

void Camera::makeModelView() {
	modelView = Math::Mat4f::translation(-position.x, -position.y, -position.z) * rotatedView;
}

void Camera::makeTransform() {
	transform = modelView * projection;
}

Vector3f Camera::getLookDirection() {
	float siny = sin(rotation.y * RADS);
	float cosy = cos(rotation.y * RADS);
	float sinx = sin(rotation.x * RADS);
	float cosx = cos(rotation.x * RADS);

	return { siny * cosx , -sinx, -cosy * cosx };
}