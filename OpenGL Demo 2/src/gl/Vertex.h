#pragma once

#include <cstdint>

struct BasicVertex
{
	static constexpr uint8_t NUM_COORD_COMPONENTS{ 3 };
	static constexpr uint8_t POS_ATTRIBUTE{ 0 };
	
	float x, y, z;
};

struct ColoredVertex : public BasicVertex
{
	static constexpr uint8_t NUM_COLOR_COMPONENTS{ 4 };
	static constexpr uint8_t COLOR_ATTRIBUTE{ 1 };

	float r, g, b, a;
};

struct TexturedVertex : public BasicVertex
{
	static constexpr uint8_t NUM_TEXTURE_COMPONENTS{ 2 };
	static constexpr uint8_t TEXTURE_ATTRIBUTE{ 2 };

	float s, t;
};

struct ColoredTexturedVertex : public ColoredVertex, TexturedVertex
{

};