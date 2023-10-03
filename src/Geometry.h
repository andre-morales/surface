#pragma once
#include "types.h"

namespace Geometry {
	namespace Quad {
		constexpr const float verts2_tris[] = {
			-.5, -.5,
			 .5, -.5,
			 .5,  .5,			 
			-.5,  .5,
		};
		constexpr const float verts3_tris[] = {
			-.5, -.5, 0,
			 .5, -.5, 0,
			 .5,  .5, 0,
			-.5,  .5, 0,
		};
		extern const float uvs[];
		extern const int16 uvs_s[];
	}
	namespace Cube {
		extern const byte indices[];
		extern const float verts_tris[];
		extern const float normals_tris[];
		extern const float uvs_tris[];
		extern const float verts_lines[];

		constexpr const int8 normals_tris_i8[] = {
			// Top
			0, 127, 0,
			0, 127, 0,
			0, 127, 0,
			0, 127, 0,

			// Bottom
			0, -128, 0,
			0, -128, 0,
			0, -128, 0,
			0, -128, 0,

			//Left
			-128, 0, 0,
			-128, 0, 0,
			-128, 0, 0,
			-128, 0, 0,

			//Right
			127, 0, 0,
			127, 0, 0,
			127, 0, 0,
			127, 0, 0,

			//Front
			0, 0, -128,
			0, 0, -128,
			0, 0, -128,
			0, 0, -128,

			//Back
			0, 0, 127,
			0, 0, 127,
			0, 0, 127,
			0, 0, 127,
		};
	}
	namespace Cube_inside {
		extern const float verts[];
		extern const float uvs[];
	}
}