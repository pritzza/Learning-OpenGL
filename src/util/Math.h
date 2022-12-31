#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Math
{
public:
	enum class Axis
	{
		X,
		Y,
		Z
	};

	static constexpr glm::vec3 I_HAT{ 1.f, 0.f, 0.f };
	static constexpr glm::vec3 J_HAT{ 0.f, 1.f, 0.f };
	static constexpr glm::vec3 K_HAT{ 0.f, 0.f, 1.f };

	static constexpr glm::mat4 IDENTITY_MATRIX{ glm::mat4(1.0f) };

};