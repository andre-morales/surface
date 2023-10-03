#pragma once
#pragma warning (disable : 4244) 
#include "vec3.h"

namespace Math {
	template<class T> union Vector4 {
		T vec[4];
		struct { T x, y, z, w; };

		Vector4() : Vector4(0) {}
		
		Vector4(T v) : x(v), y(v), z(v), w(v) {}
		
		Vector4(T x, T y, T z) : x(x), y(y), z(z), w(0) {}
		
		Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		
		template<class K>
		Vector4(const Vector3<K>& v, T w) {
			vec[0] = v.vec[0];
			vec[1] = v.vec[1];
			vec[2] = v.vec[2];
			vec[3] = w;
		}

		template<class K>
		Vector4(const Vector3<K>& v) : Vector4(v, 0) {}

		Vector4<T> operator*(T s) const {
			return Vector4<T>(x * s, y * s, z * s, w * s);
		}
		
		Vector4<T> operator+(const Vector4<T>& v) const {
			return Vector4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		operator const float* () const {
			return vec;
		}
	};
}

using Vector4i = Math::Vector4<int>;
using Vector4f = Math::Vector4<float>;
using Vector4d = Math::Vector4<double>;