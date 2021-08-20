#pragma once

#include <glm/glm.hpp>

#include "gl/Model.h"

class Entity
{
private:
	Model model;

	glm::vec3 pos{};
	glm::vec3 rotation{};
	float scaleFactor{ 1.f };

public:
	Entity(const Model& model)
		:
		model{ model }
	{}

	void setPos(const float x, const float y, const float z)				{ this->pos =  glm::vec3(x, y, z); }
	void move(const float x, const float y, const float z)					{ this->pos += glm::vec3(x, y, z); }

	void setRotation(const float pitch, const float yaw, const float roll)	{ this->rotation =  glm::vec3(pitch, yaw, roll); }
	void rotate(const float pitch, const float yaw, const float roll)		{ this->rotation += glm::vec3(pitch, yaw, roll); }

	void setScale(const float scale)										{ this->scaleFactor =  scaleFactor; }
	void scale(const float scale)											{ this->scaleFactor *= scaleFactor; }

	const Model& getModel() const											{ return this->model;		}
	const glm::vec3 getPos() const											{ return this->pos;			}
	const glm::vec3 getRotation() const										{ return this->rotation;	}
	const float getScale() const											{ return this->scaleFactor;	}
};