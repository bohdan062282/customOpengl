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
	
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	unsigned int getId();

	void setUniformInt(const std::string& name, int value) const;
	void setUniformFloat(const std::string& name, float value) const;

private:

	unsigned int shaderProgramId;

	void checkCompileErrors(unsigned int id, std::string type);
};

#endif
