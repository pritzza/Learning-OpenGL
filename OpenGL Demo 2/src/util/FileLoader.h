#pragma once

#include <cstdint>
#include <string>
#include <vector>

class FileLoader
{
private:
	std::ifstream getFile(const std::string& fileName) const;

public:
	const std::string getContents(const std::string& fileName) const;
	const std::vector<float> getVertices(const std::string& fileName) const;
	const std::vector<uint32_t> getIndices(const std::string& fileName) const;

};