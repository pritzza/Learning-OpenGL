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
	const uint32_t vertHandle{ this->makeShader(GL_VERTEX_SHADER,	vertexFile)   };
	const uint32_t fragHandle{ this->makeShader(GL_FRAGMENT_SHADER, fragmentFile) };

	glAttachShader(this->handle, vertHandle);
	glAttachShader(this->handle, fragHandle);
	glLinkProgram (this->handle);

	checkStatus(GL_LINK_STATUS, this->handle, "shader program");

	// we can free the vertex and fragment shaders
	glDeleteShader(vertHandle);
	glDeleteShader(fragHandle);
}

void ShaderProgram::use() const
{
	glUseProgram(this->handle);
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

const uint32_t ShaderProgram::getHandle() const
{
	return this->handle;
}