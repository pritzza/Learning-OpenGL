#pragma once

struct Vertex
{
	static constexpr unsigned int MEMBER_COUNT{ 3 };
	static constexpr unsigned int SIZE{ sizeof(float) * MEMBER_COUNT };

	float coords[MEMBER_COUNT];

	float& x{ coords[0] };
	float& y{ coords[1] };
	float& z{ coords[2] };

	Vertex(const float x, const float y, const float z)
		:
		x{ coords[0] },
		y{ coords[1] },
		z{ coords[2] }
	{
		coords[0] = x;
		coords[1] = y;
		coords[2] = z;
	}
};