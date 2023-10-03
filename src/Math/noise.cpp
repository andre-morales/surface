#include "noise.h"
#include "prng.h"
#include <cmath>

Noise::Noise(uint64 sd) {
	PRNG rng(sd);
	for(int i = 0; i < 4096; i++){
		samples[i] = rng.gen() % 2048;
	}
}

Noise::~Noise(){}

static constexpr double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
static constexpr double lerp(double t, double a, double b) { return a + t * (b - a); }
static double grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	double u = h<8 ? x : y,
		v = h<4 ? y : h==12||h==14 ? x : z;
	return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double Noise::get(double x, double y, double z){
	uint16* p = samples;
	int X = (int)floor(x) & 2047,
		Y = (int)floor(y) & 2047,
		Z = (int)floor(z) & 2047;
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);
	double u = fade(x),
			 v = fade(y),
			 w = fade(z);
	int A = p[X]+Y, AA = p[A]+Z, AB = p[A+1]+Z,
		B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;

	return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z ),
									 grad(p[BA], x-1, y, z )),
							 lerp(u, grad(p[AB], x, y-1, z ),
									 grad(p[BB], x-1, y-1, z ))),
					 lerp(v, lerp(u, grad(p[AA+1], x, y, z-1 ),
									 grad(p[BA+1], x-1, y, z-1 )),
							 lerp(u, grad(p[AB+1], x, y-1, z-1 ),
									 grad(p[BB+1], x-1, y-1, z-1 ))));
}
