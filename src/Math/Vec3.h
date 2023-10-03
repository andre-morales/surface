#pragma once
#pragma warning (disable : 4244) 
#include <cmath>
#include <string>
#include <format>
#include <span>
#include <cassert>
#include <limits>

namespace Math {
	template <class T>
	union Vector3 {
		T vec[3];
		struct { T x, y, z; };

		Vector3() {}

		Vector3(T v) : x(v), y(v), z(v){}

		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

		template <class K>
		Vector3(const Vector3<K>& other) {
			x = other.x;
			y = other.y;
			z = other.z;
		}

		T lengthSquared() const {
			return x * x + y * y + z * z;
		}

		T length() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		Vector3<T> normalize() const {
			T len = std::sqrt(x * x + y * y + z * z);
			
			return {
				x / len, y / len, z / len
			};
		}

		Vector3<T> closestTo(std::span<const Vector3<T>> points) const {
			assert(!points.empty());

			float closestSqDistance = std::numeric_limits<float>::max();
			Vector3<T>* closestPoint = nullptr;

			for (const auto& b : points) {
				float sqLen = (b - *this).lengthSquared();
				if (sqLen < closestSqDistance) {
					closestSqDistance = sqLen;
					closestPoint = &b;
				}
			}

			return *closestPoint;
		}

		Vector3<T>& operator+=(const Vector3<T>& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3<T>& operator-=(const Vector3<T>& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		Vector3<T> operator+(T n) const {
			return { x + n, y + n, z + n };
		}
		
		Vector3<T> operator+(const Vector3<T>& v) const {
			return { x + v.x, y + v.y, z + v.z };
		}
		
		Vector3<T> operator-(const Vector3<T>& v) const {
			return { x - v.x, y - v.y, z - v.z };
		}

		Vector3<T> operator-() const {
			return { -x, -y, -z };
		}
		
		Vector3<T> operator*(T n) const {
			return { x * n, y * n, z * n };
		}
		
		Vector3<T> operator/(T n) const {
			return { x / n, y / n, z / n };
		}
		
		bool operator!=(const Vector3<T> other) const {
			return other.x != x || other.y != y || other.z != z;
		}

		std::string toString() const {
			return std::format("[{:.2f}, {:.2f}, {:.2f}]", x, y, z);
		}

		static Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b) {
			return {
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			};
		}

		static T dot(const Vector3<T>& a, const Vector3<T>& b) {
			return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
		}
	};
}

using Vector3i = Math::Vector3<int>;
using Vector3f = Math::Vector3<float>;
using Vector3d = Math::Vector3<double>;