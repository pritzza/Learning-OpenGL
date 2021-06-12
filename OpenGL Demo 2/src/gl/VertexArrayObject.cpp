#include "VertexArrayObject.h"

#include <glad/glad.h>

#include <iostream>

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& o)
{
	std::swap(vao, o.vao);
	std::swap(vbo, o.vbo);
	std::swap(ibo, o.ibo);
	std::swap(numIndices, o.numIndices);

	return *this;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& o) noexcept
	:
	vao{o.vao},
	vbo{o.vbo},
	ibo{o.ibo},
	numIndices{o.numIndices}
{
	o.vao = 0;
	o.vbo = 0;
	o.ibo = 0;
	o.numIndices = 0;
}

void VertexArrayObject::makeVAO(const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
{
	this->bind();

	bufferVertexData(vertices);
	bufferIndicesData(indices);

	this->unbind();
}

void VertexArrayObject::bufferVertexData(const std::vector<float>& vertices) const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	constexpr uint32_t VERTEX_ELEMENTS{ 3 };

	// glVertexAttribPointer(index, size, type, normalized, stride, GLvoid* pointer);
	glVertexAttribPointer(0, VERTEX_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_ELEMENTS * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void VertexArrayObject::bufferIndicesData(const std::vector<uint32_t>& indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	this->numIndices = indices.size();
}

void VertexArrayObject::draw()
{
	this->bind();

	glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, NULL);

	this->unbind();
}

void VertexArrayObject::bind() const
{
	glBindVertexArray(vao);
}

void VertexArrayObject::unbind() const 
{
	glBindVertexArray(0);
}