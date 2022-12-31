#pragma once

#include "util/Math.h"

class ModelMatrix
{
public:
	// applies transformations to transform matrix
	void update();

	// translates from current position by translation vector
	void translate(const glm::vec3& translation);
	
	// multiply current scale component wise by scale
	void scale(const glm::vec3& scaling);

	// rotate about each axis by rotation's component's in degrees
	void rotate(const glm::vec3& rotation);

	// rotate about a single axis
	void rotate(float degrees, Math::Axis axis);
	
	// setters
	void setPosition(const glm::vec3& p)	{ position = p;			}
	void setScale(const glm::vec3& s)		{ scaleTransform = s;	}
	void setRotation(const glm::vec3& r)	{ rotation = r;			}

	const glm::vec3& getPosition() const	{ return position;			}
	const glm::vec3& getScale() const		{ return scaleTransform;	}
	const glm::vec3& getRotation() const	{ return rotation;			}
	const glm::mat4& getMatrix() const		{ return transform;			}

private:
	glm::mat4 transform{ Math::IDENTITY_MATRIX };	// model matrix

	glm::vec3 position{ 0.f, 0.f, 0.f };
	glm::vec3 scaleTransform{ 1.f, 1.f, 1.f };
	glm::vec3 rotation{ 0.f, 0.f, 0.f };	// rotation along x y z axis in degrees
};