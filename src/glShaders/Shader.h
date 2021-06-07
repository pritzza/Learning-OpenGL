#pragma once

#include <string>

class Shader
{
private:
	const unsigned int m_id;
	const std::string m_name;
	const std::string m_source;

public:
	Shader(const std::string& name, const unsigned int type, const std::string& source);
	~Shader();

	void compile() const;

	const char* const getSource() const;
	const unsigned int getID() const;

};