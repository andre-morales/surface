#include "Exception.h"

Exception::Exception() : Exception::Exception("Unknown exception!") {}
Exception::Exception(const std::string& m) :
	message(m) {}
Exception::Exception(const std::string&& m) :
	message(m) {}

const char* Exception::what() const {
	return message.c_str();
}