#pragma once

struct BasicVertex
{
	float x, y, z;
};

struct ColoredVertex : public BasicVertex
{
	float r, g, b, a;
};