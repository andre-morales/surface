#pragma once
#include "MeshGenerator.h"
#include "Client/Chunk.h"
#include "Client/Session.h"
#include "Client/World.h"
#include "MeshGeneratorTables.h"
#include "StaticVector.h"
#include <array>

using namespace Math;

Vector3f computeFaceNormal(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3) {
	// Uses p2 as a new origin for p1,p3
	auto a = p3 - p2;
	auto b = p1 - p2;
	// Compute the cross product a X b to get the face normal
	return Vector3f::cross(a, b).normalize();
}


void MeshGenerator::generate(Chunk& chunk) {
	for (int x = 0; x < 16; x++) {
		for (int y = -1; y < 16; y++) {
			for (int z = 0; z < 16; z++) {
				MeshGenerator::march(chunk, x, y, z);
			}
		}
	}
}

const Vector3i& closest(const Vector3f& a, int* indices, int indicesLength) {
	float smallestLength = 1000000;
	const Vector3i* smallestVec = nullptr;

	for (int i = 0; i < indicesLength; i++) {
		int bi = indices[i];
		const Vector3f& b = Tables::verticesF[bi];
		float len = (b - a).lengthSquared();
		if (len < smallestLength) {
			smallestLength = len;
			smallestVec = &Tables::vertices[bi];
		}
	}

	return *smallestVec;
}

void MeshGenerator::march(Chunk& chunk, int x, int y, int z) {
	World& world = chunk.session.getWorld();
	std::vector<Vector3f>& triangles = chunk.vertices;
	std::vector<Vector2f>& uvs = chunk.uvs;
	std::vector<Vector3f>& normals = chunk.normals;
	std::vector<Vector3f>& colors = chunk.colors;
	auto data = chunk.blocks;

	auto getBlock = [&](int x, int y, int z) {
		if (y >= 16) return (Block)0;
		if (y < 0) return (Block)0;

		if (x >= 16 && z >= 16) {
			auto fr = world.getChunk(chunk.cx + 1, chunk.cz + 1);
			return (*fr->blocks)[x - 16][y][z - 16];
		}
		if (x >= 16) {
			auto fr = world.getChunk(chunk.cx + 1, chunk.cz);
			return (*fr->blocks)[x - 16][y][z];
		}
		if (z >= 16) {
			auto fr = world.getChunk(chunk.cx, chunk.cz + 1);
			return (*fr->blocks)[x][y][z - 16];
		}
		return (*data)[x + 0][y + 0][z + 0];
	};

	Vector3f position{ Vector3i{x, y, z} };

	int v0 = getBlock(x + 0, y + 0, z + 0) != 0;
	int v1 = getBlock(x + 0, y + 1, z + 0) != 0;
	int v2 = getBlock(x + 1, y + 1, z + 0) != 0;
	int v3 = getBlock(x + 1, y + 0, z + 0) != 0;
	int v4 = getBlock(x + 0, y + 0, z + 1) != 0;
	int v5 = getBlock(x + 0, y + 1, z + 1) != 0;
	int v6 = getBlock(x + 1, y + 1, z + 1) != 0;
	int v7 = getBlock(x + 1, y + 0, z + 1) != 0;

	int caseBits = (v7 << 7) | (v6 << 6) | (v5 << 5) | (v4 << 4) | (v3 << 3) | (v2 << 2) | (v1 << 1) | v0;
	const int* edges = Tables::triangles[caseBits];

	int edge;

	while ((edge = *edges) != -1) {
		int e1 = *edges++;
		int e2 = *edges++;
		int e3 = *edges++;

		float uv1;
		float uv2;
		float uv3;

		auto& v1 = Tables::verticesOfEdges[e1];
		{
			int vi = Tables::closestEdgeOfCases[caseBits][e1];
			auto& c = Tables::vertices[vi];

			//auto& c = closest(v1, involved.elements, involved.length);
			uv1 = getBlock(x + c.x, y + c.y, z + c.z);
		}

		auto& v2 = Tables::verticesOfEdges[e2];
		{
			int vi = Tables::closestEdgeOfCases[caseBits][e2];
			auto& c = Tables::vertices[vi];

			//auto& c = closest(v2, involved.elements, involved.length);
			uv2 = getBlock(x + c.x, y + c.y, z + c.z);
		}

		auto& v3 = Tables::verticesOfEdges[e3];
		{
			int vi = Tables::closestEdgeOfCases[caseBits][e3];
			auto& c = Tables::vertices[vi];

			//auto& c = closest(v3, involved.elements, involved.length);
			uv3 = getBlock(x + c.x, y + c.y, z + c.z);
		}

		Vector3f direction = computeFaceNormal(v1, v2, v3);
		normals.push_back(direction);
		normals.push_back(direction);
		normals.push_back(direction);

		triangles.push_back(v1 + position);
		triangles.push_back(v2 + position);
		triangles.push_back(v3 + position);

		uvs.emplace_back(uv1, uv1);
		uvs.emplace_back(uv2, uv2);
		uvs.emplace_back(uv3, uv3);

		colors.emplace_back(uv1, uv2, uv3);
		colors.emplace_back(uv1, uv2, uv3);
		colors.emplace_back(uv1, uv2, uv3);
	}
}

