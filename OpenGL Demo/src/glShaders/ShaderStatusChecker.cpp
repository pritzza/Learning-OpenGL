#include "ShaderStatusChecker.h"

#include <glad 3.3/glad.h>

#include <iostream>

ShaderStatusChecker::ShaderStatusChecker(const unsigned int statusType)
	:
	m_statusType{ statusType }
{}

void ShaderStatusChecker::checkStatus(const unsigned int shader, const std::string& shaderName)
{
	int success;
	char infoLog[MAX_INFO_LOG_LEN];

	if (this->m_statusType == GL_COMPILE_STATUS)
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	else if (this->m_statusType == GL_LINK_STATUS)
		glGetShaderiv(shader, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, MAX_INFO_LOG_LEN, NULL, infoLog);

		if (this->m_statusType == GL_COMPILE_STATUS)
			std::cout << shaderName << " shader didn't compile ...\n";
		else if (this->m_statusType == GL_LINK_STATUS)
			std::cout << shaderName << " shader program didn't link	...\n";

		std::cout << infoLog << '\n';
	}
}