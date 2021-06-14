#include "ShaderProgram.h"

#include <glad/glad.h>

#include "../util/FileLoader.h"

#include <iostream>

ShaderProgram::ShaderProgram()
	:
	handle{ glCreateProgram() }
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->handle);

	uniforms.clear();
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& o)
{
	std::swap(handle, o.handle);

	return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram&& o) noexcept
	:
	handle{ o.handle }
{
}

const uint32_t ShaderProgram::makeShader(const uint32_t shaderType, const std::string& fileName) const
{
	const uint32_t handle{ glCreateShader(shaderType) };

	FileLoader fl;

	// lots of overhead here and in FileLoader
	std::string src{ fl.getContents(fileName) };
	const char* cSrc{ src.c_str() };

	glShaderSource(handle, 1, &cSrc, NULL);
	glCompileShader(handle);

	if (!checkStatus(GL_COMPILE_STATUS, handle, fileName))
		return 0;

	return handle;
}

void ShaderProgram::makeProgram(const std::string& vertexFile, const std::string& fragmentFile) const
{
	const uint32_t vertHandle{ this->makeShader(GL_VERTEX_SHADER,	vertexFile) };
	const uint32_t fragHandle{ this->makeShader(GL_FRAGMENT_SHADER, fragmentFile) };

	glAttachShader(this->handle, vertHandle);
	glAttachShader(this->handle, fragHandle);
	glLinkProgram(this->handle);

	checkStatus(GL_LINK_STATUS, this->handle, "shader program");

	// we can free the vertex and fragment shaders
	glDeleteShader(vertHandle);
	glDeleteShader(fragHandle);
}

void ShaderProgram::use() const
{
	glUseProgram(this->handle);
}

void ShaderProgram::registerUniform(const UniformName uniformName)
{
	std::string name;
	switch (uniformName)
	{
	case UniformName::MainFragmentColorOffset:  name = "colorOffset"; break;
	case UniformName::MainVertexPosOffset:		name = "posOffset";   break;

	default:	std::cout << "Error: case for UniformName: " << static_cast<int>(uniformName) << " does not exist.\n";
	}

	Uniform uniform(glGetUniformLocation(this->handle, name.c_str()));

	if (uniform.location == -1)	// if couldnt find
		std::cout << "Error: Couldn't find \"" << name << "\" uniform.\n";
	else
		uniforms.emplace(uniformName, uniform);
}

const void ShaderProgram::setUniform(const UniformName uniform, float vector[3]) const
{
	if (isValidUniform(uniform))
		glUniform3fv(getUniform(uniform), 1, vector);
}
const void ShaderProgram::setUniform(const UniformName uniform, int vector[3]) const
{
	if (isValidUniform(uniform))
		glUniform3iv(getUniform(uniform), 1, vector);
}
/*
const void ShaderProgram::setUniform(const UniformName uniform, float matrix[4]) const
{
	if (isValidUniform(uniform))
		glUniformMatrix4fv(uniform.location, 1, false, matrix);
}
*/
const void ShaderProgram::setUniform(const UniformName uniform, int value) const
{
	if (isValidUniform(uniform))
		glUniform1i(getUniform(uniform), value);
}
const void ShaderProgram::setUniform(const UniformName uniform, unsigned  value) const
{
	if (isValidUniform(uniform))
		glUniform1ui(getUniform(uniform), value);
}
const void ShaderProgram::setUniform(const UniformName uniform, float value) const
{
	if (isValidUniform(uniform))
		glUniform1f(getUniform(uniform), value);
}

const bool ShaderProgram::checkStatus(const uint32_t checkingStatus, const uint32_t handle, const std::string& name) const
{
	int success;
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
		int32_t messageLength;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &messageLength);
		char* infoLog = (char*)alloca(messageLength * sizeof(char));	// cherno's method of stack allocating dynamic array

		glGetShaderInfoLog(handle, messageLength, NULL, infoLog);

		std::cout << "Error! " << name << " couldn't " << checkingStatusName << ":\n" << infoLog << '\n';

		return false;
	}

	return true;
}

const bool ShaderProgram::isValidUniform(const UniformName uniform) const
{
	bool isValid{ true };

	if (uniforms.find(uniform) == uniforms.end())
	{
		isValid = false;
		std::cout << "Error: Uniform " << static_cast<int>(uniform) << " not yet registered.\n";
	}
	if (!this->isInUse)
	{
		isValid = false;
		std::cout << "Error: Shader must be in use to modify uniform.\n";
	}

	return isValid;
}

const uint32_t ShaderProgram::getHandle() const
{
	return this->handle;
}

const int ShaderProgram::getUniform(const UniformName uniform) const
{
	return this->uniforms.at(uniform).location;
}
