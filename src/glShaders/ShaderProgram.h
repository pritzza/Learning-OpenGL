#pragma once

#include <string>

class Shader;

class ShaderProgram
{
private:
	const unsigned int m_id;
	const std::string m_name;

	bool m_isUsing{ false };

public:
	ShaderProgram(const std::string& name);
	~ShaderProgram();

	void add(const Shader& shader) const;
	void link() const;

	// use() moved to Renderer

	const unsigned int getID() const;
};