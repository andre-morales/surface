#include "MeshGeneratorTables.h"
#include "StaticVector.h"
#include <limits>

namespace Tables {

	static consteval std::array<Vector3f, 12> genMiddleEdges() {
		std::array<Vector3f, 12> arr;
		for (int i = 0; i < 12; i++) {
			arr[i] = Vector3f(edges[i][0] + edges[i][1]) / 2;
		}
		return arr;
	}

	static consteval std::array<std::array<int, 12>, 256> genClosestEdgeOfCases() {
		std::array<std::array<int, 12>, 256> arr{};

		for (int caseI = 0; caseI < 256; caseI++) {
			for (int edgeI = 0; edgeI < 12; edgeI++) {
				StaticVector<int, 8> involved;
				auto& vert = verticesOfEdges[edgeI];

				if (caseI & 1)   { involved.push(0); }
				if (caseI & 2)   { involved.push(1); }
				if (caseI & 4)   { involved.push(2); }
				if (caseI & 8)   { involved.push(3); }
				if (caseI & 16)  { involved.push(4); }
				if (caseI & 32)  { involved.push(5); }
				if (caseI & 64)  { involved.push(6); }
				if (caseI & 128) { involved.push(7); }
				
				float smallestLength = std::numeric_limits<float>::max();
				int smallestVec = -1;

				for (int i = 0; i < involved.length; i++) {
					int bi = involved.elements[i];
					auto& b = Tables::verticesF[bi];
					float len = (b - vert).lengthSquared();
					if (len < smallestLength) {
						smallestLength = len;
						smallestVec = bi;
					}
				}

				arr[caseI][edgeI] = smallestVec;
			}
		}
		return arr;
	}

	constexpr std::array<Vector3f, 12> verticesOfEdges = genMiddleEdges();

	constexpr std::array<std::array<int, 12>, 256> closestEdgeOfCases = genClosestEdgeOfCases();

	//size_t what = sizeof(closestEdgeOfCases);
}