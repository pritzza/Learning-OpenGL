#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

struct TransformationMatrix
{
private:
	glm::mat4 matrix;

public:
	TransformationMatrix(
		const glm::vec3 pos,
		const glm::vec3 rotation,
		const float scale
	)
		:
		matrix{ 1.f }
	{
		// translate
		this->matrix = glm::translate(this->matrix, pos);

		// rotate
		this->matrix = glm::rotate(this->matrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->matrix = glm::rotate(this->matrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->matrix = glm::rotate(this->matrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

		// uniform scale
		this->matrix = glm::scale(this->matrix, glm::vec3(scale));
	}

	inline const glm::mat4& get() const
	{
		return matrix;
	}
};