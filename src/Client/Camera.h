#pragma once
#include "Math/Matrices.h"
#include "Math/Vecs.h"
#include <array>
#include <optional>

class Camera {
public:
	Vector3f position{};
	Vector2f rotation{};
	Vector3f velocity{};

	Math::Mat4f projectionMat, rotatedViewMat, modelViewMat, fullTransformMat;

	float aspect = 4.0 / 3.0;
	float fov = 90;
	float near = 0.1;
	float far = 1000;

	bool isLookingAtTerrain = false;
	Vector3f lookingTerrainPos;
	std::array<Vector3f, 3> lookingTerrainTriangle;

	Camera();

	/* Update camera transform matrices with new position, rotation or frustum parameters */
	void doTransformMatrices();

	/* Perform a new raycast against the terrain and update looked at information */
	void doCameraRaycast();

	/* Perform camera velocity and position updates. Used to smooth freecaming movement */
	void doMotionUpdate(float delta);

	Vector3f getLookingDirection();
	std::optional<Vector3f> getLookingWorldPos();
	std::optional<Vector3i> getLookingBlockPos();
	std::optional<Vector3i> getLookingEmptyBlockPos();

private:
	Camera(const Camera&) = delete;
	Camera(Camera&&) = delete;
};