#include "Shader.h"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string VSCodeString;
	std::string FSCodeString;
	std::ifstream VSFile;
	std::ifstream FSFile;

	VSFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	FSFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		VSFile.open(vertexShaderPath);
		FSFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << VSFile.rdbuf();
		fragmentShaderStream << FSFile.rdbuf();

		VSFile.close();
		FSFile.close();

		VSCodeString = vertexShaderStream.str();
		FSCodeString = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << "\n -- --------------------------------------------------- -- " << std::endl;
	}
	const char* VSCode = VSCodeString.c_str();
	const char* FSCode = FSCodeString.c_str();

	unsigned int VS, FS;

	VS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VS, 1, &VSCode, NULL);
	glCompileShader(VS);
	checkCompileErrors(VS, "VS");

	FS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FS, 1, &FSCode, NULL);
	glCompileShader(FS);
	checkCompileErrors(FS, "FS");

	shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, VS);
	glAttachShader(shaderProgramId, FS);
	glLinkProgram(shaderProgramId);
	checkCompileErrors(shaderProgramId, "PROGRAM");

	glDeleteShader(VS);
	glDeleteShader(FS);
}

unsigned int Shader::getId()
{
	return shaderProgramId;
}

void Shader::setUniformInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramId, name.c_str()), value);
}
void Shader::setUniformFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgramId, name.c_str()), value);
}

void Shader::checkCompileErrors(unsigned int id, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
