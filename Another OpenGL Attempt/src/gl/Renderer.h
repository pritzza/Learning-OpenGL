#pragma once

#include <glad/glad.h>

struct Model;

class Renderer
{
	static constexpr GLfloat CLEAR_COLOR[4] = { 0.2f, 0.3f, 0.3f, 1.0f };

public:
	void render(const Model& model) const;

	void clear() const;
};