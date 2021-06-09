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