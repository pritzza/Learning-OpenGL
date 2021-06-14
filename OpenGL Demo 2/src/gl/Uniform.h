#pragma once

enum class UniformName
{
	// there should be one value here for each enum in every shader in the program.
	// ideally each uniform should include the name of its corresponding program, shader and identifier

	MainFragmentColorOffset,

	MainVertexPosOffset
};

struct Uniform
{
	static constexpr int INVALID{ -1 };

	const int location;

	Uniform(const int location);
};