#pragma once
#include "Math/Matrices.h"
#include "Math/Vecs.h"
#include <array>
#include <optional>

class Camera {
public:
	Vector3f position = Vector3f(0, 0, 2);
	Vector2f rotation = Vector2f(0, 0);

	Math::Mat4f projectionMat, rotatedViewMat, modelViewMat, fullTransformMat;

	float aspect = 4.0 / 3.0;
	float fov = 90;
	float near = 0.1;
	float far = 1000;

	bool isLookingAtTerrain = false;
	Vector3f lookingTerrainPos;
	std::array<Vector3f, 3> lookingTerrainTriangle;

	Camera();
	void makeTransformMatrices();
	void doCameraRaycast();

	Vector3f getLookingDirection();
	std::optional<Vector3f> getLookingWorldPos();
	std::optional<Vector3i> getLookingBlockPos();
	std::optional<Vector3i> getLookingEmptyBlockPos();

private:
	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
};