#pragma once

struct Color
{
	float r, g, b, a;

	Color(const float r, const float g, const float b, const float a)
		:
		r{r},
		g{g},
		b{b},
		a{a}
	{}

	Color(const float rgba[4])
		:
		r{ rgba[0] },
		g{ rgba[1] },
		b{ rgba[2] },
		a{ rgba[3] }
	{}
};