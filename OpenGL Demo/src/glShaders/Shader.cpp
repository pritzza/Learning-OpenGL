#include "Shader.h"

#include "ShaderStatusChecker.h"

#include <glad 3.3/glad.h>
#include <iostream>

Shader::Shader(const std::string& name, const unsigned int type, const std::string& source)
	:
	m_id{ glCreateShader(type) },
	m_name{ name },
	m_source{ source }
{
	this->compile();
}

Shader::~Shader()
{
	glDeleteShader(this->m_id);
}

void Shader::compile() const
{
	const char* const cStringSource{ this->m_source.c_str() };

	glShaderSource(this->m_id, 1, &cStringSource, NULL);
	glCompileShader(this->m_id);


	ShaderStatusChecker statusChecker(GL_COMPILE_STATUS);
	statusChecker.checkStatus(this->m_id, this->m_name);
}

const char* const Shader::getSource() const
{
	return this->m_name.c_str();
}

const unsigned int Shader::getID() const
{
	return this->m_id;
}
