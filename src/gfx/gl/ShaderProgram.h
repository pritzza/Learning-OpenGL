#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "GLTypeDefs.h"

#include <glm/mat4x4.hpp>

struct Material;
struct MappedMaterial;
struct Light;

class ShaderProgram
{
private:
	static inline constexpr GLuint NULL_HANDLE{ 0 };
	static inline constexpr bool PRINT_ERRORS{ true };

private:
	GLuint handle{ NULL_HANDLE };
	std::unordered_map<std::string_view, GLint> uniforms;

public:
	ShaderProgram() = default;
	ShaderProgram(
		const std::string_view& vertexShaderPath,
		const std::string_view& fragmentShaderPath,
		const std::vector<std::string_view>& uniformNames
	);

	~ShaderProgram();

	void init(
		const std::string_view& vertexShaderPath,
		const std::string_view& fragmentShaderPath,
		const std::vector<std::string_view>& uniformNames
	);

	void setUniformf(const std::string_view& name, float value);
	void setUniformi(const std::string_view& name, int value);
	void setUniformVec3f(const std::string_view& name, const glm::vec3& v);
	void setUniformMat4(const std::string_view& name, const glm::mat4& matrix);

	void setUniformMaterial(
		const std::string_view& shininess,
		const std::string_view& ambient,
		const std::string_view& diffuse,
		const std::string_view& specular,
		const Material& mat
	);

	void setUniformMaterial(
		const std::string_view& shininess,
		const std::string_view& diffuse,
		const std::string_view& specular,
		const MappedMaterial& mat,
		GLuint diffuseMapTextureUnit,
		GLuint specularMapTextureUnit
	);

	void setUniformLight(
		const std::string_view& isPoint,
		const std::string_view& position,
		const std::string_view& direction,
		const std::string_view& innerCone,
		const std::string_view& outterCone,
		const std::string_view& ambient,
		const std::string_view& diffuse,
		const std::string_view& specular,
		const std::string_view& attenuationCoeff,
		const Light& light
	);

	void use();

	const GLuint get() const;

private:

	void createProgram(GLuint vertex, GLuint fragment);

	const GLuint createShader(
		GLuint shaderType, 
		const std::string_view& sourcePath
	) const;

	void checkStatus(
		GLuint statusType, 
		GLuint handle, 
		const std::string_view& name
	) const;

	void findUniforms(const std::vector<std::string_view>& uniformNames);

};