#pragma once

#include "glad/glad.h"

#include <string>
#include <vector>

// i want model to be more of a struct who's data is manipulated by others, rather than itself performing operations
struct Model
{
private:
	static constexpr int VERTEX_ELEMENTS{ 5 };
	static constexpr int VERTEX_POS_ELEMENTS{ 3 };
	static constexpr int VERTEX_UV_ELEMENTS{ 2 };

	static constexpr int VERTEX_POS_ATTRIB{ 0 };
	static constexpr int VERTEX_UV_ATTRIB{ 1 };
	static constexpr int VERTEX_NORMAL_ATTRIB{ 2 };

private:
	GLuint vao, vbo, ibo, texture;

	GLuint vertexCount;

private:
	void bufferVertices(const std::vector<GLfloat>& vertices);
	void bufferIndices(const std::vector<GLuint>& indices);
	void bufferTexture(const std::string& vertices);

public:
	Model() = default;
	Model(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData, const std::string& textureName);

	~Model();

	void init(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData, const std::string& textureName);

	const GLuint getVertexCount() const { return this->vertexCount; }
	const GLuint getTexture() const		{ return this->texture;     }
	const GLuint getVAO() const			{ return this->vao;         }
};