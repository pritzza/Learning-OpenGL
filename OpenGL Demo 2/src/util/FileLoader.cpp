#include "FileLoader.h"

#include <iostream>
#include <fstream>

// getting lots of overhead by not being able to return these by ref
std::ifstream FileLoader::getFile(const std::string& fileName) const
{
	std::ifstream file;

	file.open(fileName);

	if (file.fail())
		std::cout << "Can't find or open \"" << fileName << "\"\n";

	return file;
}

const std::string FileLoader::getContents(const std::string& fileName) const
{
	std::ifstream file{ getFile(fileName) };

	std::string contents{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	return contents;
}

const std::vector<float> FileLoader::getVertices(const std::string& fileName) const
{
	std::vector<float> vertices;

	std::ifstream file{ getFile(fileName) };
	std::string line;

	constexpr uint8_t OBJ_LINE_OFFSET = std::string_view("v ").length();

	while (std::getline(file, line))
	{
		if (line[0] == 'v')
		{
			// eg input for line: 
			// v -3.4101800e-003 1.3031957e-001 2.1754370e-002
			const std::string x{ line.substr(2, line.find_first_of(' ', 2) - 2) };	// get num between "v " and second space
			const std::string y{ line.substr(2 + x.length() + 1, x.length()) };		// get num between "v " + x.len + " " and the third space
			const std::string z{ line.substr(2 + (x.length() * 2) + 1) };			// get num between "v " + x.len + " " + y.len + " " to the end

			vertices.push_back(std::stof(x));
			vertices.push_back(std::stof(y));
			vertices.push_back(std::stof(z));

			//std::cout << std::stof(x) << ", " << std::stof(y) << ", " << std::stof(z) << '\n';
		}
	}

	return vertices;
}

const std::vector<uint32_t> FileLoader::getIndices(const std::string& fileName) const
{
	std::vector<uint32_t> indices;

	std::ifstream file{ getFile(fileName) };
	std::string line;

	constexpr uint8_t OBJ_LINE_OFFSET = std::string_view("f ").length();

	while (std::getline(file, line))
	{
		if (line[0] == 'f')
		{
			// this is kindof a mess
			// eg input for line: 
			// f 1320 2443 2503
			// get the numbers between "f " and the second space
			const std::string x{ line.substr(2, line.find_first_of(' ', 2) - 2) };
			// get the numbers between "f " + xDigits.len + " " to the third space
			const std::string y{ line.substr(2 + x.length() + 1, line.substr(2 + x.length() + 1).find_first_of(' ')) };
			// get the numbers between "f " + xDigits.len + " " + yDigits.len + " " to the end of line
			const std::string z{ line.substr(2 + x.length() + y.length() + 2) };

			indices.push_back(std::stoi(x));
			indices.push_back(std::stoi(y));
			indices.push_back(std::stoi(z));

			//std::cout << std::stof(x) << ", " << std::stof(y) << ", " << std::stof(z) << '\n';
		}
	}

	return indices;
}
