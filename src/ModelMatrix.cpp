#include "ModelMatrix.h"

#include <glm/gtc/matrix_transform.hpp>

void ModelMatrix::update()
{
	// reset transform
	transform = Math::IDENTITY_MATRIX;

	// scale
	transform = glm::scale(transform, scaleTransform);

	// rotate
	transform = glm::rotate(transform, rotation.x, Math::I_HAT);
	transform = glm::rotate(transform, rotation.y, Math::J_HAT);
	transform = glm::rotate(transform, rotation.z, Math::K_HAT);

	// translate
	transform = glm::translate(transform, position);
}

void ModelMatrix::translate(const glm::vec3& translation)
{
	position += translation;
}

void ModelMatrix::scale(const glm::vec3& scaling)
{
	scaleTransform *= scaling;
}

void ModelMatrix::rotate(const glm::vec3& rotation)
{
	this->rotation += rotation;
}

void ModelMatrix::rotate(float degrees, Math::Axis axis)
{
	switch (axis)
	{
		case Math::Axis::X: rotation.x += degrees; break;
		case Math::Axis::Y: rotation.y += degrees; break;
		case Math::Axis::Z: rotation.z += degrees; break;
	}
}
