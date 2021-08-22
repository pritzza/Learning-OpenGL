#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class MatrixMaker
{
public:
	const glm::mat4 makeTransformationMatrix(const glm::vec3 pos, const glm::vec3 rotation, const float scale) const
	{
		// create identity matrix 
		glm::mat4 matrix{ 1.f };

		// translate
		matrix = glm::translate(matrix, pos);

		// rotate
		matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

		// uniform scale
		matrix = glm::scale(matrix, glm::vec3(scale));

		return matrix;
	}

	const glm::mat4 makeProjectionMatrix(const int width, const int height, const int fov, const float near, const float far) const
	{
		glm::mat4 matrix{ 0.f };

		const float aspectRatio{ (float)width / height };

		// MATH!!!!!
		const float yScale{ 1.f / glm::tan(glm::radians(fov / 2.f)) * aspectRatio };
		const float xScale{ yScale / aspectRatio };

		const float frustumLength{ far - near };

		// MATH!!!!!
		matrix[0][0] = xScale;
		matrix[1][1] = yScale;
		matrix[2][2] = -((far + near) / frustumLength);
		matrix[2][3] = -1;
		matrix[3][2] = -((2.f * far * near) / frustumLength);
		matrix[3][3] = 0;

		return matrix;
	}

	const glm::mat4 makeViewMatrix(const glm::vec3 pos, const glm::vec3 rotation) const
	{
		// create identity matrix 
		glm::mat4 matrix{ 1.f };

		// rotate
		matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

		// translate
		matrix = glm::translate(matrix, -pos);

		return matrix;
	}
};