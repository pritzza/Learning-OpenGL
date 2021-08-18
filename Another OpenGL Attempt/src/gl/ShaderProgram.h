#pragma once

#include <glad/glad.h>

#include <string>
#include <unordered_map>

class ShaderProgram
{
public:
	enum class ShaderType
	{
		Vertex,
		Fragment
	};

private:
	GLuint program{ };

	std::unordered_map<std::string, GLint> uniforms;

private:
	const bool checkStatus(const GLuint checkingStatus, const GLuint handle, const std::string& name) const;

	const GLuint getShader(const std::string& fileName, const ShaderType type) const;

public:
	ShaderProgram() = default;
	ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

	~ShaderProgram();

	void init(const std::string& vertexShader, const std::string& fragmentShader);

	void setUniform(const std::string& uniformName, const GLfloat x, const GLfloat y, const GLfloat z);
	void setUniform(const std::string& uniformName, const GLuint textureID);

	inline void use()
	{
		glUseProgram(this->program);
	}
};