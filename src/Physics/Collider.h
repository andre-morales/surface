#pragma once
#include "Pointers.h"

class btCollisionShape;

namespace Physics {
	class Collider {
	public:
		Collider(unique<btCollisionShape>&& shape);
		virtual ~Collider();

		btCollisionShape& getBtShape() const;

	protected:
		unique<btCollisionShape> collisionShape;

		Collider();

	private:
		Collider(Collider&&) = delete;
		Collider(const Collider&) = delete;
	};
}