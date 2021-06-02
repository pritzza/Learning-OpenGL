#include "ShaderProgram.h"

#include "Shader.h"
#include "ShaderStatusChecker.h"

#include <glad 3.3/glad.h>

ShaderProgram::ShaderProgram(const std::string& name)
	:
	m_id{ glCreateProgram() },
	m_name{ name }
{}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->m_id);
}

void ShaderProgram::add(const Shader& shader) const
{
	glAttachShader(this->m_id, shader.getID());
}

void ShaderProgram::link() const
{
	glLinkProgram(this->m_id);


	ShaderStatusChecker statusChecker(GL_LINK_STATUS);
	statusChecker.checkStatus(this->m_id, this->m_name);
}

void ShaderProgram::use() const
{
	glUseProgram(this->m_id);
}

const unsigned int ShaderProgram::getID() const
{
	return this->m_id;
}
