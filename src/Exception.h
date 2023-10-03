#pragma once
#include <exception>
#include <string>

class Exception : public std::exception {
public:
	const std::string message;

	Exception();
	Exception(const std::string&);
	Exception(const std::string&&);

	virtual const char* what() const override;
};