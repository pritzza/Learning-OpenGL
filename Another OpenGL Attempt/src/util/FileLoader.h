#pragma once

#include <cstdint>
#include <string>
#include <vector>

class FileLoader final
{
private:
	std::ifstream getFile(const std::string& fileName) const;

public:
	const std::string getContents(const std::string& fileName) const;

};