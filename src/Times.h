#pragma once
#include <chrono>

namespace Time {
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> Point;
	typedef std::chrono::duration<std::chrono::high_resolution_clock> Duration;

	static inline Point now() {
		return std::chrono::high_resolution_clock::now();
	}

	static inline double elapsedBetween(Point stime, Point etime) {
		return std::chrono::duration_cast<std::chrono::microseconds>(etime - stime).count() * 0.000001;
	}

	template <class T, class U>
	static inline double toMillis(std::chrono::duration<T, U> time) {
		return std::chrono::duration_cast<std::chrono::microseconds>(time).count() * 0.000001;
	}

	static inline double elapsedSince(Point stime) {
		return elapsedBetween(stime, now());
	}
}