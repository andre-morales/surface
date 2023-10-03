#include "prng.h"
PRNG::PRNG(int _seed){
	this->seed = _seed;
	this->curseed = _seed;
}
PRNG::PRNG(uint64 _seed){
	this->seed = _seed;
	this->curseed = _seed;
}
uint64 PRNG::gen(){
    curseed = (8253729 * curseed + 2396403);
    return curseed % 32767;
}
