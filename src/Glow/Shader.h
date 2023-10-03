#pragma once
#include "Math/matrices.h"
#include "Math/vec2.h"
#include "types.h"
#include <string>
#include <unordered_map>

class Shader {
public:
	bool dirty; // If this shader needs to be recompiled.
	uint32 glId;
	/*std::unordered_map<std::string, std::string> constants;
	std::unordered_map<std::string, int32> uniformLocations;
	std::unordered_map<std::string, int32> attribsLocations;*/

	Shader();
	~Shader();

	void readShaderFile (std::string_view path);
	/*int32 getUniformID(const std::string&);
	int32 getAttributeID(const std::string&);
	void setUniform(const char*, int);
	void setUniform(const char*, float);
	void setUniform(const char*, const mat4f&);
	void setUniform(const char*, const mat4d&);
	void setUniform(const char*, const vec2i&);
	void setUniform(const char*, const vec2f&);
	void setUniform(const char*, bool);*/
	void compile();
	/*void setConstant (const std::string&, bool);
	void setConstant(const std::string&, const std::string&);
	void unsetConstant(const std::string&);*/

private:
	std::string source;
	std::string glVertStr, glFragStr;
	uint32 glVertId, glFragId;

	void destroy();
	bool buildShader(uint32, uint32, const std::string&);
};
