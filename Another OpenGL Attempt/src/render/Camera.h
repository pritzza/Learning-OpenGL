#pragma once

#include <glm/glm.hpp>

struct Camera
{
private:
	glm::vec3 pos{};
	glm::vec3 rotation{};

public:
	void setPos(const float x, const float y, const float z)				{ this->pos =  glm::vec3(x, y, z); }
	void move(const float x, const float y, const float z)					{ this->pos += glm::vec3(x, y, z); }

	void setRotation(const float pitch, const float yaw, const float roll)	{ this->rotation =  glm::vec3(pitch, yaw, roll); }
	void rotate(const float pitch, const float yaw, const float roll)		{ this->rotation += glm::vec3(pitch, yaw, roll); }

	const glm::vec3 getPos() const											{ return this->pos;			}
	const glm::vec3 getRotation() const										{ return this->rotation;	}
};