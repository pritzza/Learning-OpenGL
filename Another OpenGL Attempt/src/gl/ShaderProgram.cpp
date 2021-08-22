#include "ShaderProgram.h"

#include "../math/MatrixMaker.h"

#include "../util/FileLoader.h"

#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	this->init(vertexShader, fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

void ShaderProgram::init(const std::string& vertexShader, const std::string& fragmentShader)
{
	// we have the creation of the indiivdual shaders abstracted, so this is really just the creation
	// of the shader program, could or couldnt be made into a function idk
	this->program = glCreateProgram();

	const GLuint vertexID{ this->getShader(vertexShader, ShaderType::Vertex) };
	const GLuint fragmentID{ this->getShader(fragmentShader, ShaderType::Fragment) };

	glAttachShader(program, vertexID);
	glAttachShader(program, fragmentID);

	glLinkProgram(this->program);
	checkStatus(GL_LINK_STATUS, this->program, "shader program");

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

const GLuint ShaderProgram::getShader(const std::string& fileName, const ShaderType type) const
{
	// some prep work
	GLuint shaderTypeID{};
	std::string shaderTypeIDName{};

	switch (type)
	{
	case ShaderType::Vertex:	shaderTypeID = GL_VERTEX_SHADER;	shaderTypeIDName = "Vertex Shader";		break;
	case ShaderType::Fragment:	shaderTypeID = GL_FRAGMENT_SHADER;	shaderTypeIDName = "Fragment Shader";	break;	
	}

	// getting the shader's source from disk
	const FileLoader fl;
	const std::string shaderSourceString{ fl.getShaderContents(fileName) };
	const char* shaderSource{ shaderSourceString.c_str() };

	// actual opengl
	const GLuint shaderID{ glCreateShader(shaderTypeID) };

	glShaderSource(shaderID, 1, &shaderSource, NULL);

	glCompileShader(shaderID);
	checkStatus(GL_COMPILE_STATUS, shaderID, shaderTypeIDName);

	return shaderID;
}

void ShaderProgram::loadUniform(const std::string& uniformName)
{
	// if we dont have a handle to the uniform, get it
	if (uniforms.find(uniformName) == uniforms.end())
		uniforms.insert({ uniformName, glGetUniformLocation(this->program, uniformName.c_str()) });
}

void ShaderProgram::setUniform(const std::string& uniformName, const GLfloat x, const GLfloat y, const GLfloat z)
{
	loadUniform(uniformName);
	glUniform3f(uniforms.at(uniformName), x, y, z);
}

void ShaderProgram::setUniform(const std::string& uniformName, const GLuint textureID)
{
	loadUniform(uniformName);
	glUniform1i(uniforms.at(uniformName), textureID);
}

void ShaderProgram::setUniform(const std::string& uniformName, const glm::mat4& matrix)
{
	loadUniform(uniformName);
	glUniformMatrix4fv(uniforms.at(uniformName), 1, GL_FALSE, &matrix[0][0]);
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
