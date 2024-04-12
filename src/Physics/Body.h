#pragma once
namespace Physics {
	class Body {
	public:
		const float mass;
		const float isConvex;
		const int tag;

		Body(float mass, bool isConvex, int tag);
	};
}

