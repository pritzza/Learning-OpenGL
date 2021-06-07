#pragma once

#include <string>

struct ShaderStatusChecker
{
private:
	static constexpr int MAX_INFO_LOG_LEN{ 512 };

private:
	const unsigned int m_statusType;

public:
	ShaderStatusChecker(const unsigned int statusType);

	void checkStatus(const unsigned int shader, const std::string& shaderName);
};