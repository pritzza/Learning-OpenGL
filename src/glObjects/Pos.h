#pragma once

struct Pos
{
	static constexpr int SIZE{ 3 };
	const float x, y, z;

	Pos(const float x, const float y, const float z)
		:
		x{x},
		y{y},
		z{z}
	{};

	Pos(const float coords[3])
		:
		x{ coords[0] },
		y{ coords[1] },
		z{ coords[2] }
	{};
};