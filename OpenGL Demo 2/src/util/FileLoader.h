#pragma once

#include <string>

class FileLoader
{
private:
	std::ifstream getFile(const std::string& fileName) const;

public:
	const std::string getContents(const std::string& fileName) const;
};