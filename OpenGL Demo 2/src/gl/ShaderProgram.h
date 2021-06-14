#pragma once

#include <cstdint>

#include <string>
#include <unordered_map>

#include "Uniform.h"

class ShaderProgram final
{
private:
	uint32_t handle;
	bool isInUse;
	std::unordered_map<UniformName, Uniform> uniforms;

private:
	const uint32_t makeShader(const uint32_t shaderType, const std::string& fileName) const;

	const bool checkStatus(const uint32_t checkingStatus, const uint32_t handle, const std::string& name) const;

	const bool isValidUniform(const UniformName uniform) const;

	const int getUniform(const UniformName uniform) const;

public:
	ShaderProgram();
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&&);

	ShaderProgram(ShaderProgram&&) noexcept;

	void makeProgram(const std::string& vertexFile, const std::string& fragmentFile) const;

	void use() const;

	void registerUniform(const UniformName uniformName);

	const void setUniform(const UniformName uniform, float vector[3]) const;
	const void setUniform(const UniformName uniform, int   vector[3]) const;
	//const void setUniform(const UniformName uniform, float matrix[4]) const;
	const void setUniform(const UniformName uniform, int       value) const;
	const void setUniform(const UniformName uniform, unsigned  value) const;
	const void setUniform(const UniformName uniform, float     value) const;

	const uint32_t getHandle() const;
};