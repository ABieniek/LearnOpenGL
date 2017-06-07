#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use() {glUseProgram(program);};
	// uniform setting functions
	void setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
	};
	void setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(program, name.c_str()), value);
	};
	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(program, name.c_str()), value);
	};
};

#endif