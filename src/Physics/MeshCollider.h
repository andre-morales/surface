#pragma once
#include "Collider.h"

class btTriangleMesh;
class btBvhTriangleMeshShape;

namespace Physics {
	class MeshCollider : public Collider {
	public:
		MeshCollider(unique<btTriangleMesh>&&);
		virtual ~MeshCollider();

		btTriangleMesh& getBtMesh() const;
		btBvhTriangleMeshShape& getBtShape() const;

	private:
		unique<btTriangleMesh> mesh;
		btBvhTriangleMeshShape* shape;
	};
}

