#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

class ShaderProgram
{
private:
	static inline constexpr GLuint INVALID_HANDLE = -1;
	static inline constexpr bool PRINT_ERRORS{ true };

private:
	GLuint handle{ INVALID_HANDLE };
	std::unordered_map<std::string_view, GLint> uniforms;

public:
	ShaderProgram(
		std::string_view vertexShaderPath,
		std::string_view fragmentShaderPath,
		const std::vector<std::string_view>& uniformNames
	);

	~ShaderProgram();

	void setUniformf(std::string_view name, float value);
	void setUniformi(std::string_view name, int value);
	void setUniformMat4(std::string_view name, const glm::mat4& matrix);

	const GLuint get() const;

private:
	void createProgram(GLuint vertex, GLuint fragment);

	const GLuint createShader(
		GLuint shaderType, 
		std::string_view sourcePath
	) const;

	void checkStatus(
		GLuint statusType, 
		GLuint handle, 
		std::string_view name
	) const;

	void findUniforms(const std::vector<std::string_view>& uniformNames);

};

