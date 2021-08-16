#pragma once

#include <glad/glad.h>

#include <string>

class ShaderProgram
{
private:
	GLuint program{ };

private:
	const bool checkStatus(const GLuint checkingStatus, const GLuint handle, const std::string& name) const;

public:
	ShaderProgram() = default;
	ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);

	void init(const std::string& vertexShader, const std::string& fragmentShader);

	inline void use()
	{
		glUseProgram(this->program);
	}
};