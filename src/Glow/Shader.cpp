#include "Shader.h"
#include "GL/glew.h"
#include "Exception.h"
#include <sstream>
#include <fstream>
#include <stack>

Shader::Shader(){
	glId = glVertId = glFragId = 0;
	dirty = true;
}

Shader::~Shader(){
	destroy();
}

void Shader::readShaderFile(std::string_view file){
	bool frag = true, vert = true;
	std::ostringstream src_oss;
	std::ostringstream v_oss, f_oss;

	std::ifstream is(std::string(file), std::ifstream::in);
	if(is.fail()) throw Exception("File read error.");

	char buff[512];
	while(!is.eof()){
		is.getline(buff, 512);
		src_oss << buff << '\n';
		if(strcmp(buff, "#vert") == 0){
			vert = true;
			frag = false;
			continue;
		}
		if(strcmp(buff, "#frag") == 0){
			frag = true;
			vert = false;
			continue;
		}

		if(vert) v_oss << buff << '\n';
		if(frag) f_oss << buff << '\n';

	}
	glVertStr = v_oss.str();
	glFragStr = f_oss.str();
	is.close();
}
/*
int32 Shader::getUniformID(const std::string& uniform){
	auto it = uniformLocations.find(uniform);
	if(it != uniformLocations.end()) return it->second;

	int32 id = glGetUniformLocation(glId, uniform.c_str());
	uniformLocations.emplace(uniform, id);
	return id;
}

int32 Shader::getAttributeID(const std::string& attrib){
	auto it = attribsLocations.find(attrib);
	if(it != attribsLocations.end()) return it->second;

	int32 id = glGetAttribLocation(glId, attrib.c_str());
	attribsLocations.emplace(attrib, id);
	return id;
}

void Shader::setUniform(const char* uniform, int i){
	glUniform1i(getUniformID(uniform), i);
}

void Shader::setUniform(const char* uniform, float f){
	glUniform1f(getUniformID(uniform), f);
}

void Shader::setUniform(const char* uniform, bool b){
	glUniform1i(getUniformID(uniform), b);
}

void Shader::setUniform(const char* uniform, const mat4f& mat){
	int32 id = getUniformID(uniform);
	if(id != -1) glUniformMatrix4fv(id, 1, false, mat);
}

void Shader::setUniform(const char* uniform, const mat4d& mat){
	int32 id = getUniformID(uniform);
	if(id != -1) glUniformMatrix4fv(id, 1, false, mat4f(mat));
}

void Shader::setUniform(const char* uniform, const vec2i& vec){
	glUniform2iv(getUniformID(uniform), 1, vec.vec);
}

void Shader::setUniform(const char* uniform, const vec2f& vec){
	glUniform2fv(getUniformID(uniform), 1, vec.vec);
}*/

void Shader::compile(){
	destroy();
	glId = glCreateProgram();
	glVertId = glCreateShader(GL_VERTEX_SHADER);
	if(!buildShader(glId, glVertId, glVertStr)){
		char infoLog[1024];
		int infoLogSize;
		glGetShaderInfoLog(glVertId, 1024, &infoLogSize, infoLog);
		printf("Vertex shader failed!\n%s\n", infoLog);
	}

	glFragId = glCreateShader(GL_FRAGMENT_SHADER);
	if(!buildShader(glId, glFragId, glFragStr)){
		char infoLog[1024];
		int infoLogSize;
		glGetShaderInfoLog(glFragId, 1024, &infoLogSize, infoLog);
		printf("Fragment shader failed!\n%s\n", infoLog);
	}

	glLinkProgram(glId);
	GLint linkStatus = 0;
	glGetProgramiv(glId, GL_LINK_STATUS, &linkStatus);
	if(!linkStatus){
		GLchar infoLog[1024];
		GLint infoLogSize;
		glGetProgramInfoLog(glId, 1024, &infoLogSize, infoLog);
		printf("Shader link failed!\n%s\n", infoLog);
	}
	//uniformLocations.clear();
}
/*
std::string trimBegin(const std::string& str){
	int startpos = str.find_first_not_of(" \t");
	if(string::npos != startpos){
		return str.substr(startpos);
	}
	return str;
}*/

bool Shader::buildShader(uint32 program, uint32 shader, const std::string& srcStr){
	std::ostringstream oss;
	bool reading = true; // Will be false inside a #if CONST that results in false.
	std::stack<bool> ifStack;
	//for(auto& constant : constants){
	//	oss << "#define " << constant.first << " " << constant.second << "\n";
	//}
	oss << srcStr;

	const std::string str = oss.str();
	const char* src[] = {str.c_str()};
	glShaderSource(shader, 1, src, nullptr);
	glCompileShader(shader);
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE){
		return false;
	}
	glAttachShader(program, shader);
	dirty = false;
	return true;
}

/*
void Shader::setConstant(const std::string& constant, const std::string& value){
	constants[constant] = value;
	dirty = true;
}

void Shader::setConstant(const std::string& constant, bool value){
	const std::string _value = (value)?"1":"0";
	setConstant(constant, _value);
}

void Shader::unsetConstant(const std::string& constant){
	constants.erase(constant);
	dirty = true;
}*/

void Shader::destroy(){
	if(glVertId) glDeleteShader(glVertId);
	if(glFragId) glDeleteShader(glFragId);
	if(glId) glDeleteProgram(glId);
}