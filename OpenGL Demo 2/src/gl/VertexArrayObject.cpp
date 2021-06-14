#include "VertexArrayObject.h"

#include <glad/glad.h>

#include "Vertex.h"

#include <SFML/Graphics/Image.hpp>

#include <iostream>

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenTextures(1, &texture);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteTextures(1, &texture);
}

VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& o)
{
	std::swap(vao, o.vao);
	std::swap(vbo, o.vbo);
	std::swap(ibo, o.ibo);
	std::swap(numIndices, o.numIndices);
	std::swap(texture, o.texture);

	return *this;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& o) noexcept
	:
	vao{ o.vao },
	vbo{ o.vbo },
	ibo{ o.ibo },
	numIndices{ o.numIndices },
	texture{ o.texture }
{
	o.vao = 0;
	o.vbo = 0;
	o.ibo = 0;
	o.numIndices = 0;
	o.texture = 0;
}

void VertexArrayObject::bufferVertexData(const std::vector<float>& vertices) const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	constexpr uint32_t VERTEX_ELEMENTS{ 3 };

	// glVertexAttribPointer(index, size, type, normalized, stride, GLvoid* pointer);
	glVertexAttribPointer(BasicVertex::POS_ATTRIBUTE, VERTEX_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_ELEMENTS * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(BasicVertex::POS_ATTRIBUTE);
}

void VertexArrayObject::bufferVertexData(const std::vector<BasicVertex>& vertices) const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(BasicVertex), vertices.data(), GL_STATIC_DRAW);

	// glVertexAttribPointer(index, size, type, normalized, stride, GLvoid* pointer);
	glVertexAttribPointer(BasicVertex::POS_ATTRIBUTE, BasicVertex::NUM_COORD_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), (void*)0);
	
	glEnableVertexAttribArray(BasicVertex::POS_ATTRIBUTE);
}

void VertexArrayObject::bufferVertexData(const std::vector<ColoredVertex>& vertices) const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ColoredVertex), vertices.data(), GL_STATIC_DRAW);

	// glVertexAttribPointer(index, size, type, normalized, stride, GLvoid* pointer);
	glVertexAttribPointer(BasicVertex::POS_ATTRIBUTE, BasicVertex::NUM_COORD_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*)0);
	glVertexAttribPointer(ColoredVertex::COLOR_ATTRIBUTE, ColoredVertex::NUM_COLOR_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (void*) sizeof(BasicVertex));
	
	glEnableVertexAttribArray(BasicVertex::POS_ATTRIBUTE);
	glEnableVertexAttribArray(ColoredVertex::COLOR_ATTRIBUTE);
}

void VertexArrayObject::bufferVertexData(const std::vector<TexturedVertex>& vertices) const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedVertex), vertices.data(), GL_STATIC_DRAW);

	// glVertexAttribPointer(index, size, type, normalized, stride, GLvoid* pointer);
	glVertexAttribPointer(BasicVertex::POS_ATTRIBUTE, BasicVertex::NUM_COORD_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
	glVertexAttribPointer(TexturedVertex::TEXTURE_ATTRIBUTE, TexturedVertex::NUM_TEXTURE_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(BasicVertex));
	
	glEnableVertexAttribArray(BasicVertex::POS_ATTRIBUTE);
	glEnableVertexAttribArray(TexturedVertex::TEXTURE_ATTRIBUTE);
}

void VertexArrayObject::bufferVertexData(const std::vector<ColoredTexturedVertex>& vertices) const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ColoredTexturedVertex), vertices.data(), GL_STATIC_DRAW);

	// glVertexAttribPointer(index, size, type, normalized, stride, GLvoid* pointer);
	glVertexAttribPointer(BasicVertex::POS_ATTRIBUTE, BasicVertex::NUM_COORD_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(ColoredTexturedVertex), (void*)0);
	glVertexAttribPointer(ColoredVertex::COLOR_ATTRIBUTE, ColoredVertex::NUM_COLOR_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(ColoredTexturedVertex),       (void*) (sizeof(BasicVertex)));
	glVertexAttribPointer(TexturedVertex::TEXTURE_ATTRIBUTE, TexturedVertex::NUM_TEXTURE_COMPONENTS, GL_FLOAT, GL_FALSE, sizeof(ColoredTexturedVertex), (void*) (sizeof(ColoredVertex)));

	glEnableVertexAttribArray(BasicVertex::POS_ATTRIBUTE);
	glEnableVertexAttribArray(ColoredVertex::COLOR_ATTRIBUTE);
	glEnableVertexAttribArray(TexturedVertex::TEXTURE_ATTRIBUTE);
}

void VertexArrayObject::bufferIndicesData(const std::vector<uint32_t>& indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	this->numIndices = indices.size();
}

void VertexArrayObject::bufferTextureData(const sf::Image& texture)
{
	glBindTexture(GL_TEXTURE_2D, this->texture);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexImage2D(target, level, internalFormat, width, height, border, format, type, data)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

	glGenerateMipmap(GL_TEXTURE_2D);
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