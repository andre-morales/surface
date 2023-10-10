#pragma once
#include "Ray.h"
#include "Plane.h"
#include <optional>
#include "AABB.h"

class Collisions {
public:
	static std::optional<Vector3f> raycastTriangle(const Ray& ray, const Vector3f triangle[]);

    static bool IsIntersecting(AABB& box, const Vector3f triangle[]);

   /* interface IVector {
        double X{ get; }
        double Y{ get; }
        double Z{ get; }
        double[] Coords{ get; }
        double Dot(IVector other);
        IVector Minus(IVector other);
        IVector Cross(IVector other);
    }

    interface IShape {
        IEnumerable<IVector> Vertices{ get; }
    }

    interface IAABox : IShape {
        IVector Start{ get; }
        IVector End{ get; }
    }

    interface ITriangle : IShape {
        IVector Normal{ get; }
        IVector A{ get; }
        IVector B{ get; }
        IVector C{ get; }
    }*/

private:
	static bool isPointInTriangle(const Vector3f& point, const Vector3f triangle[]);
	static std::optional<Vector3f> raycastPlaneNoNormal(const Ray& ray, const Plane& plane);
};

