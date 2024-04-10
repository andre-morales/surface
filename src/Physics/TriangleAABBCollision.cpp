#include "Collisions.h"
#include <limits>
#include <span>
#include "Loggy.h"

// ---- AABB Intersection implementation by Markus Jarderot -- Test 1 ----
Vector3f normalFromTriangle(const Vector3f triangle[]) {
    const auto& a = triangle[0];
    const auto& b = triangle[1];
    const auto& c = triangle[2];
    return Vector3f::cross(b - a, c - a).normalize();
}

void Project(std::span<const Vector3f> points, Vector3f axis, float* min, float* max);

bool Collisions::isIntersecting(const AABB& box, const Vector3f triangle[]) {
    auto [boxMinV, boxMaxV] = box.getMinMax();

    std::span<const Vector3f> triangleSpan{ triangle, 3 };
    float triangleMin, triangleMax;
    float boxMin, boxMax;

    // Test the box normals (x-, y- and z-axes)
    Vector3f boxNormals[]{
        Vector3f(1,0,0),
        Vector3f(0,1,0),
        Vector3f(0,0,1)
    };

    for (int i = 0; i < 3; i++) {
        //  auto n = boxNormals[i];
        Project(triangleSpan, boxNormals[i], &triangleMin, &triangleMax);

        if (triangleMax < boxMinV.vec[i] || triangleMin > boxMaxV.vec[i])
            return false; // No intersection possible.
    }

    // Test the triangle normal

    //double triangleOffset = triangle.Normal.Dot(triangle.A);
    Vector3f triangleNormal = normalFromTriangle(triangle);
    double triangleOffset = Vector3f::dot(triangleNormal, triangle[0]);

    //Project(box.Vertices, triangle.Normal, out boxMin, out boxMax);
    Project(box.getVertices(), triangleNormal, &boxMin, &boxMax);
    if (boxMax < triangleOffset || boxMin > triangleOffset)
        return false; // No intersection possible.

    // Test the nine edge cross-products
    Vector3f triangleEdges[]{
        triangle[0] - triangle[1],
        triangle[1] - triangle[2],
        triangle[2] - triangle[0]
    };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            // The box normals are the same as it's edge tangents
            Vector3f axis = Vector3f::cross(triangleEdges[i], boxNormals[j]);
            Project(box.getVertices(), axis, &boxMin, &boxMax);
            Project(triangleSpan, axis, &triangleMin, &triangleMax);
            if (boxMax < triangleMin || boxMin > triangleMax)
                return false; // No intersection possible
        }

    // No separating axis found.
    return true;
}

void Project(std::span<const Vector3f> points, Vector3f axis, float* min, float* max)
{
    *min = std::numeric_limits<float>::max();
    *max = std::numeric_limits<float>::lowest();

    for (auto& p : points) {
        float val = Vector3f::dot(axis, p);
        if (val < *min) *min = val;
        if (val > *max) *max = val;
    }
}
