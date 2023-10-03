#pragma once
#include "Vecs.h"
#include <cmath>
#include <format>

namespace Math {
	template <class T>
	union Mat4x4 {
		T mat[16];
		struct {
			T m00, m10, m20, m30;
			T m01, m11, m21, m31;
			T m02, m12, m22, m32;
			T m03, m13, m23, m33;
		};

		/* Constucts a mat4 with an identity matrix already loaded in. */
		Mat4x4() : Mat4x4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1) {}

		/* Constucts a mat4 copying the 16 values necessary from the pointer. */
		Mat4x4(const T* _mat) {
			memcpy(mat, _mat, 16 * sizeof(T));
		}

		/* Constucts a mat4 given each value manually. */
		Mat4x4(
			T m00, T m10, T m20, T m30,
			T m01, T m11, T m21, T m31,
			T m02, T m12, T m22, T m32,
			T m03, T m13, T m23, T m33) :
			m00(m00), m10(m10), m20(m20), m30(m30),
			m01(m01), m11(m11), m21(m21), m31(m31),
			m02(m02), m12(m12), m22(m22), m32(m32),
			m03(m03), m13(m13), m23(m23), m33(m33) {}

		/* Constructs a mat4 copying the values from another mat4. */
		template <class K>
		Mat4x4(const Mat4x4<K>& m) :
			m00(m.m00), m10(m.m10), m20(m.m20), m30(m.m30),
			m01(m.m01), m11(m.m11), m21(m.m21), m31(m.m31),
			m02(m.m02), m12(m.m12), m22(m.m22), m32(m.m32),
			m03(m.m03), m13(m.m13), m23(m.m23), m33(m.m33) {}

		Vector4<T> operator*(const Vector4<T>& v) const {
			return Vector4f{
				m00 * v.x + m01 * v.y + m02 * v.z + m03 * v.w,
				m10 * v.x + m11 * v.y + m12 * v.z + m13 * v.w,
				m20 * v.x + m21 * v.y + m22 * v.z + m23 * v.w,
				m30 * v.x + m31 * v.y + m32 * v.z + m33 * v.w
			};
		}

		template <class K>
		Mat4x4<K> operator*(const Mat4x4<K>& m) const {
			return Mat4x4<K>(
				m00 * m.m00 + m10 * m.m01 + m20 * m.m02 + m30 * m.m03,
				m00 * m.m10 + m10 * m.m11 + m20 * m.m12 + m30 * m.m13,
				m00 * m.m20 + m10 * m.m21 + m20 * m.m22 + m30 * m.m23,
				m00 * m.m30 + m10 * m.m31 + m20 * m.m32 + m30 * m.m33,

				m01 * m.m00 + m11 * m.m01 + m21 * m.m02 + m31 * m.m03,
				m01 * m.m10 + m11 * m.m11 + m21 * m.m12 + m31 * m.m13,
				m01 * m.m20 + m11 * m.m21 + m21 * m.m22 + m31 * m.m23,
				m01 * m.m30 + m11 * m.m31 + m21 * m.m32 + m31 * m.m33,

				m02 * m.m00 + m12 * m.m01 + m22 * m.m02 + m32 * m.m03,
				m02 * m.m10 + m12 * m.m11 + m22 * m.m12 + m32 * m.m13,
				m02 * m.m20 + m12 * m.m21 + m22 * m.m22 + m32 * m.m23,
				m02 * m.m30 + m12 * m.m31 + m22 * m.m32 + m32 * m.m33,

				m03 * m.m00 + m13 * m.m01 + m23 * m.m02 + m33 * m.m03,
				m03 * m.m10 + m13 * m.m11 + m23 * m.m12 + m33 * m.m13,
				m03 * m.m20 + m13 * m.m21 + m23 * m.m22 + m33 * m.m23,
				m03 * m.m30 + m13 * m.m31 + m23 * m.m32 + m33 * m.m33
			);
		}

		Mat4x4<T>& mul(const Mat4x4<T>& m) {
			T a00 = m00;
			T a10 = m10;
			T a20 = m20;
			//T a30 = m30;
			m00 = m00 * m.m00 + m10 * m.m01 + m20 * m.m02 + m30 * m.m03;
			m10 = a00 * m.m10 + m10 * m.m11 + m20 * m.m12 + m30 * m.m13;
			m20 = a00 * m.m20 + a10 * m.m21 + m20 * m.m22 + m30 * m.m23;
			m30 = a00 * m.m30 + a10 * m.m31 + a20 * m.m32 + m30 * m.m33;

			T a01 = m01;
			T a11 = m11;
			T a21 = m21;
			//T a31 = m31;
			m01 = m01 * m.m00 + m11 * m.m01 + m21 * m.m02 + m31 * m.m03;
			m11 = a01 * m.m10 + m11 * m.m11 + m21 * m.m12 + m31 * m.m13;
			m21 = a01 * m.m20 + a11 * m.m21 + m21 * m.m22 + m31 * m.m23;
			m31 = a01 * m.m30 + a11 * m.m31 + a21 * m.m32 + m31 * m.m33;

			T a02 = m02;
			T a12 = m12;
			T a22 = m22;
			//T a32 = m32;
			m02 = m02 * m.m00 + m12 * m.m01 + m22 * m.m02 + m32 * m.m03;
			m12 = a02 * m.m10 + m12 * m.m11 + m22 * m.m12 + m32 * m.m13;
			m22 = a02 * m.m20 + a12 * m.m21 + m22 * m.m22 + m32 * m.m23;
			m32 = a02 * m.m30 + a12 * m.m31 + a22 * m.m32 + m32 * m.m33;

			T a03 = m03;
			T a13 = m13;
			T a23 = m23;
			//T a33 = m33;
			m03 = m03 * m.m00 + m13 * m.m01 + m23 * m.m02 + m33 * m.m03;
			m13 = a03 * m.m10 + m13 * m.m11 + m23 * m.m12 + m33 * m.m13;
			m23 = a03 * m.m20 + a13 * m.m21 + m23 * m.m22 + m33 * m.m23;
			m33 = a03 * m.m30 + a13 * m.m31 + a23 * m.m32 + m33 * m.m33;
			return *this;
		}
		// X rotation
		static Mat4x4<T> rotationX(double angle) {
			double c = std::cos(angle);
			double s = std::sin(angle);
			return Mat4x4<T>(
				1, 0, 0, 0,
				0, c, s, 0,
				0, -s, c, 0,
				0, 0, 0, 1
			);
		}

		// Y rotation
		static Mat4x4<T> rotationY(double angle) {
			double c = std::cos(angle);
			double s = std::sin(angle);
			return Mat4x4<T>(
				c, 0, -s, 0,
				0, 1, 0, 0,
				s, 0, c, 0,
				0, 0, 0, 1
			);
		}

		// Translation
		static Mat4x4<T> translation(double x, double y, double z) {
			return Mat4x4<T>(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				x, y, z, 1
			);
		}

		// Scale
		static Mat4x4<T> scaling(double x, double y, double z) {
			return Mat4x4<T>(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
			);
		}

		// Perspective Projection
		static Mat4x4<T> perspective(float aspect, float fov, float near, float far) {
			double h = 1.0 / std::tan(3.14 / 360.0 * fov);
			double neg_depth = near - far;

			return Mat4x4<T>(
				h / aspect, 0, 0, 0,
				0, h, 0, 0,
				0, 0, (far + near) / neg_depth, -1,
				0, 0, 2.0 * (near * far) / neg_depth, 0
			);
		}

		static Mat4x4<T> ortho(float l, float r, float b, float t, float n, float f) {
			return Mat4x4<T>(
				2 / (r - l), 0, 0, -(r + l) / (r - l),
				0, 2 / (t - b), 0, -(t + b) / (t - b),
				0, 0, -2 / (f - n), -(f + n) / (f - n),
				0, 0, 0, 1
			);
		}

		Mat4x4<T> transpose() const {
			return Mat4x4<T>(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33);
		}

		Mat4x4<T> inverse() const {
			auto m = this->mat;

			Mat4x4 dst;
			float* inv = dst.mat;

			inv[0] = m[5] * m[10] * m[15] -
				m[5] * m[11] * m[14] -
				m[9] * m[6] * m[15] +
				m[9] * m[7] * m[14] +
				m[13] * m[6] * m[11] -
				m[13] * m[7] * m[10];

			inv[4] = -m[4] * m[10] * m[15] +
				m[4] * m[11] * m[14] +
				m[8] * m[6] * m[15] -
				m[8] * m[7] * m[14] -
				m[12] * m[6] * m[11] +
				m[12] * m[7] * m[10];

			inv[8] = m[4] * m[9] * m[15] -
				m[4] * m[11] * m[13] -
				m[8] * m[5] * m[15] +
				m[8] * m[7] * m[13] +
				m[12] * m[5] * m[11] -
				m[12] * m[7] * m[9];

			inv[12] = -m[4] * m[9] * m[14] +
				m[4] * m[10] * m[13] +
				m[8] * m[5] * m[14] -
				m[8] * m[6] * m[13] -
				m[12] * m[5] * m[10] +
				m[12] * m[6] * m[9];

			inv[1] = -m[1] * m[10] * m[15] +
				m[1] * m[11] * m[14] +
				m[9] * m[2] * m[15] -
				m[9] * m[3] * m[14] -
				m[13] * m[2] * m[11] +
				m[13] * m[3] * m[10];

			inv[5] = m[0] * m[10] * m[15] -
				m[0] * m[11] * m[14] -
				m[8] * m[2] * m[15] +
				m[8] * m[3] * m[14] +
				m[12] * m[2] * m[11] -
				m[12] * m[3] * m[10];

			inv[9] = -m[0] * m[9] * m[15] +
				m[0] * m[11] * m[13] +
				m[8] * m[1] * m[15] -
				m[8] * m[3] * m[13] -
				m[12] * m[1] * m[11] +
				m[12] * m[3] * m[9];

			inv[13] = m[0] * m[9] * m[14] -
				m[0] * m[10] * m[13] -
				m[8] * m[1] * m[14] +
				m[8] * m[2] * m[13] +
				m[12] * m[1] * m[10] -
				m[12] * m[2] * m[9];

			inv[2] = m[1] * m[6] * m[15] -
				m[1] * m[7] * m[14] -
				m[5] * m[2] * m[15] +
				m[5] * m[3] * m[14] +
				m[13] * m[2] * m[7] -
				m[13] * m[3] * m[6];

			inv[6] = -m[0] * m[6] * m[15] +
				m[0] * m[7] * m[14] +
				m[4] * m[2] * m[15] -
				m[4] * m[3] * m[14] -
				m[12] * m[2] * m[7] +
				m[12] * m[3] * m[6];

			inv[10] = m[0] * m[5] * m[15] -
				m[0] * m[7] * m[13] -
				m[4] * m[1] * m[15] +
				m[4] * m[3] * m[13] +
				m[12] * m[1] * m[7] -
				m[12] * m[3] * m[5];

			inv[14] = -m[0] * m[5] * m[14] +
				m[0] * m[6] * m[13] +
				m[4] * m[1] * m[14] -
				m[4] * m[2] * m[13] -
				m[12] * m[1] * m[6] +
				m[12] * m[2] * m[5];

			inv[3] = -m[1] * m[6] * m[11] +
				m[1] * m[7] * m[10] +
				m[5] * m[2] * m[11] -
				m[5] * m[3] * m[10] -
				m[9] * m[2] * m[7] +
				m[9] * m[3] * m[6];

			inv[7] = m[0] * m[6] * m[11] -
				m[0] * m[7] * m[10] -
				m[4] * m[2] * m[11] +
				m[4] * m[3] * m[10] +
				m[8] * m[2] * m[7] -
				m[8] * m[3] * m[6];

			inv[11] = -m[0] * m[5] * m[11] +
				m[0] * m[7] * m[9] +
				m[4] * m[1] * m[11] -
				m[4] * m[3] * m[9] -
				m[8] * m[1] * m[7] +
				m[8] * m[3] * m[5];

			inv[15] = m[0] * m[5] * m[10] -
				m[0] * m[6] * m[9] -
				m[4] * m[1] * m[10] +
				m[4] * m[2] * m[9] +
				m[8] * m[1] * m[6] -
				m[8] * m[2] * m[5];

			float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

			if (det == 0)
				// not invertible
				return mat;

			for (int i = 0; i < 16; i++) {
				inv[i] /= det;
			}

			return dst;
		}

		operator const T* () const {
			return mat;
		}

		std::string toString() {
			return std::format("\n{}, {}, {}, {}\n{}, {}, {}, {}\n{}, {}, {}, {}\n{}, {}, {}, {}\n",
				m00, m01, m02, m03,
				m10, m11, m12, m13, 
				m20, m21, m22, m23, 
				m30, m31, m32, m33);
		}
	};

	using Mat4f = Mat4x4<float>;
}