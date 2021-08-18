#include "FileLoader.h"

#include <iostream>
#include <fstream>

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

	return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

const std::string FileLoader::getShaderContents(const std::string& fileName) const
{
	return getContents(this->SHADERS_DIR + fileName);
}

const sf::Image FileLoader::getImage(const std::string& fileName) const
{
	sf::Image image;

	if (!image.loadFromFile(this->TEXTURES_DIR + fileName))
		image.create(1, 1);

	return image;
}
