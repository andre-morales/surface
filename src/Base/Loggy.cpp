#include "Loggy.h"

namespace Loggy {
	void init() {}
	void print() {}

	Logger::Logger() : Logger("-") {}

	Logger::Logger(const std::string& b) : branch(b) {}
}