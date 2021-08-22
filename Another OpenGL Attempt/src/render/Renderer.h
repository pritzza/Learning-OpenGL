#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "Camera.h"

struct Model;
class Entity;
class ShaderProgram;

class Renderer
{
private:
	static constexpr GLfloat CLEAR_COLOR[4] = { 0.2f, 0.3f, 0.3f, 1.0f };

private:
	const int width;
	const int height;

	int fov{ 90 };
	float near{ 0.1f };
	float far{ 100.f };

	glm::mat4 projectionMatrix;

	Camera camera;

public:
	Renderer(const int width, const int height);

	void render(ShaderProgram& shader, const Entity& entity) const;

	void clear() const;

	inline Camera& getCamera() { return this->camera; }
};