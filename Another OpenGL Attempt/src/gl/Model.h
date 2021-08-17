#pragma once

#include "glad/glad.h"

#include <string>
#include <vector>

// i want model to be more of a struct who's data is manipulated by others, rather than itself performing operations
struct Model
{
private:
	static constexpr int VERTEX_POS_ATTRIB{ 0 };
	static constexpr int VERTEX_UV_ATTRIB{ 1 };
	static constexpr int VERTEX_NORMAL_ATTRIB{ 2 };

private:
	GLuint vao, vbo, ibo, texture;

	GLuint vertexCount;

private:
	void bufferData() const;

public:
	Model() = default;
		
	Model(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData, const std::string& textureName);

	void init(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData, const std::string& textureName);

	const GLuint getVertexCount() const { return this->vertexCount; }
	const GLuint getTexture() const		{ return this->texture;     }
	const GLuint getVAO() const			{ return this->vao;         }
};