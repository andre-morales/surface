#include "Collision.h"

// --- Clip collision using collision information as implemented by Wicked Engine, way smoother
// https://wickedengine.net/capsule-collision-detection/
void Collision::correctMotion(Vector3f& position, Vector3f& velocity) {
	auto nVelocity = velocity.normalize();
	auto velocityM = velocity.length();
	auto undesired = normal * Vector3f::dot(nVelocity, normal);
	auto desired = nVelocity - undesired;

	velocity = desired * velocityM;
	position += normal * (penetration + 0.0001f);
}

void Collision::correctGravityMotion(Vector3f& position, Vector3f& velocity) {
	//if (abs(Vector3f::dot(normal, { 0, 1, 0 })) > 0) {
		velocity = { 0, 0, 0 };
	//}

	auto nVelocity = velocity.normalize();
	auto velocityM = velocity.length();
	auto undesired = normal * Vector3f::dot(nVelocity, normal);
	auto desired = nVelocity - undesired;

	velocity = desired * velocityM;
	velocity = { 0, 0, 0 };
	position += normal * (penetration + 0.0001f);
}