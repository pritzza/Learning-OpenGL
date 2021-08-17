#include "ShaderProgram.h"

#include "../util/FileLoader.h"

#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	this->init(vertexShader, fragmentShader);
}

void ShaderProgram::init(const std::string& vertexShader, const std::string& fragmentShader)
{
	const FileLoader fl;

	const std::string vertexSourceString{ fl.getContents(vertexShader) };
	const std::string fragmentSourceString{ fl.getContents(fragmentShader) };

	const char* vertexSource{ vertexSourceString.c_str() };
	const char* fragmentSource{ fragmentSourceString.c_str() };

	this->program = glCreateProgram();

	const GLuint vertexID{ glCreateShader(GL_VERTEX_SHADER) };
	const GLuint fragmentID{ glCreateShader(GL_FRAGMENT_SHADER) };

	glShaderSource(vertexID, 1, &vertexSource, NULL);
	glShaderSource(fragmentID, 1, &fragmentSource, NULL);

	glCompileShader(vertexID);
	checkStatus(GL_COMPILE_STATUS, vertexID, "vertex shader");

	glCompileShader(fragmentID);
	checkStatus(GL_COMPILE_STATUS, fragmentID, "fragment shader");

	glAttachShader(program, vertexID);
	glAttachShader(program, fragmentID);

	glLinkProgram(this->program);
	checkStatus(GL_LINK_STATUS, this->program, "shader program");

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void ShaderProgram::setUniform(const std::string& uniformName, const GLfloat x, const GLfloat y, const GLfloat z)
{
	// if we dont have a handle to the uniform, get it
	if (uniforms.find(uniformName) == uniforms.end())
		uniforms.insert({ uniformName, glGetUniformLocation(this->program, uniformName.c_str()) });

	glUniform3f(uniforms.at(uniformName), x, y, z);
}

void ShaderProgram::setUniform(const std::string& uniformName, const GLuint textureID)
{
	if (uniforms.find(uniformName) == uniforms.end())
		uniforms.insert({ uniformName, glGetUniformLocation(this->program, uniformName.c_str()) });

	glUniform1i(uniforms.at(uniformName), textureID);
}

const bool ShaderProgram::checkStatus(const GLuint checkingStatus, const GLuint handle, const std::string& name) const
{
	GLint success;
	std::string checkingStatusName;

	switch (checkingStatus)
	{
	case GL_COMPILE_STATUS:
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		checkingStatusName = "compile";
		break;
	case GL_LINK_STATUS:
		glGetShaderiv(handle, GL_LINK_STATUS, &success);
		checkingStatusName = "link";
		break;
	}

	if (!success)
	{
		GLint messageLength;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageLength);
		char* infoLog = (char*)alloca(messageLength * sizeof(char));	// cherno's method of stack allocating dynamic array

		glGetShaderInfoLog(handle, messageLength, NULL, infoLog);

		std::cout << "Error! " << name << " couldn't " << checkingStatusName << ":\n" << infoLog << '\n';

		return false;
	}

	return true;
}