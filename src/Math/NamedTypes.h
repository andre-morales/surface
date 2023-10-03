#pragma once
#include "Vecs.h"

namespace Math {
	template <class T>
	union Rect {
		T arr[4];
		struct { T x, y, w, h; };

		Rect(T x_, T y_, T w_, T h_) :
			x(x_), y(y_), w(w_), h(h_) {}

		Rect(const Vector4<T>& v) :
			x(v.x), y(v.y), w(v.w), h(v.h) {}

		bool contains(const Vector2<T>& p) {
			return (p.x >= x && p.x <= x + w
				&& p.y >= y && p.y <= y + h);
		}
	};

	template <class T>
	union Insets {
		T arr[4];
		struct { T top, right, bottom, left; };

		Insets() :
			top(0), right(0), bottom(0), left(0) {}

		Insets(T top, T right, T bottom, T left) :
			top(top), right(right), bottom(bottom), left(left) {}

		explicit operator bool() const {
			return (top != 0) && (right != 0) && (bottom != 0) && (left != 0);
		}
	};

	template <class T>
	union Color {
		T arr[4];
		struct { T r, g, b, a; };
	};

	template <class T>
	union Size {
		T arr[2];
		struct { T w, h; };
	};

	using Pointf = Vector2f;
	using Sizei = Size<int>;
	using Sizef = Size<float>;
	using Rectf = Rect<float>;
		
	using Insetsf = Insets<float>;
	using Colorf = Color<float>;
}
