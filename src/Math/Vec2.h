#pragma once
#pragma warning (disable : 4244) 
#include <cmath>

namespace Math {
	template <class T>
	union Vector2 {
		T vec[2];
		struct { T x, y; };

		constexpr Vector2() : x(0), y(0) {}

		Vector2(T x, T y) : x(x), y(y) {}

		T length() const {
			return std::sqrt(x * x + y * y);
		}

		Vector2<T>& operator-=(const Vector2<T>& v) {
			x -= v.x;
			y -= v.y;
			return *this;
		}

		Vector2<T> operator+(T n) const {
			return { x + n, y + n };
		}

		Vector2<T> operator+(const Vector2<T>& v) const {
			return { x + v.x, y + v.y };
		}

		Vector2<T> operator-(const Vector2<T>& v) const {
			return { x - v.x, y - v.y };
		}

		Vector2<T> operator-() const {
			return { -x, -y };
		}

		Vector2<T> operator*(const T n) const {
			return { x * n, y * n };
		}

		bool operator==(const Vector2<T>& other) const {
			return other.x == x && other.y == y;
		}

		bool operator!=(const Vector2<T>& other) const {
			return other.x != x || other.y != y;
		}
	};
}

using Vector2i = Math::Vector2<int>;
using Vector2f = Math::Vector2<float>;
using Vector2d = Math::Vector2<double>;