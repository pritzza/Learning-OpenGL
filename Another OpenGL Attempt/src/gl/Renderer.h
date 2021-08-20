#pragma once

#include <glad/glad.h>

class Entity;
struct Model;
class ShaderProgram;

class Renderer
{
private:
	static constexpr GLfloat CLEAR_COLOR[4] = { 0.2f, 0.3f, 0.3f, 1.0f };

public:
	void render(ShaderProgram& shader, const Entity& entity) const;

	void clear() const;
};