#include "MeshCollider.h"
#include <bullet/btBulletDynamicsCommon.h>

namespace Physics {
	MeshCollider::~MeshCollider() {}

	MeshCollider::MeshCollider(unique<btTriangleMesh>&& m) 
		: mesh(std::move(m)){
		
		shape = new btBvhTriangleMeshShape(mesh.get(), true);
		collisionShape.reset(shape);
	}

	btTriangleMesh& MeshCollider::getBtMesh() const {
		return *mesh;
	}

	btBvhTriangleMeshShape& MeshCollider::getBtShape() const {
		return *shape;
	}
}