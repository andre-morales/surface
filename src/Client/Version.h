#pragma once
#include <string>
#include <format>
#define BUILD_NUMBER 34

constexpr int VERSION_MAJOR = 0;
constexpr int VERSION_MINOR = 0;
constexpr int VERSION_PATCH = 3;
constexpr unsigned long VERSION_BUILD = BUILD_NUMBER;

static std::string VERSION_STRING = std::format("{}.{}.{} b{}", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_BUILD);
static std::string BUILD_STRING = VERSION_STRING + " Early Dev 1";
static std::string BUILD_TEXT = "Surface v" + BUILD_STRING;
