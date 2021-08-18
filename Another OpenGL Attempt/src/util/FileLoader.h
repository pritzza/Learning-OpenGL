#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SFML/Graphics/Image.hpp>

class FileLoader final
{
private:
	inline static const std::string RES_DIR{ "Data/" };

	inline static const std::string OBJS_DIR	{ RES_DIR + "Objs/"		};
	inline static const std::string SHADERS_DIR { RES_DIR + "Shaders/"  };
	inline static const std::string TEXTURES_DIR{ RES_DIR + "Textures/" };

private:
	std::ifstream getFile(const std::string& fileName) const;

public:
	const std::string getContents(const std::string& fileName) const;
	const std::string getShaderContents(const std::string& fileName) const;
	const sf::Image getImage(const std::string& fileName) const;

};