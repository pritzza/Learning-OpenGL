#pragma once

#include <cstdint>

#include <string>

class ShaderProgram
{
private:
	uint32_t handle;

private:
	const uint32_t makeShader(const uint32_t shaderType, const std::string& fileName) const;

	const bool checkStatus(const uint32_t checkingStatus, const uint32_t handle, const std::string& name) const;

public:
	ShaderProgram();
	~ShaderProgram();

	void makeProgram(const std::string& vertexFile, const std::string& fragmentFile) const;

	void use() const;

	const uint32_t getHandle() const;
};