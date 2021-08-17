#include "Model.h"

#include <SFML/Graphics/Image.hpp>

#include <iostream>

Model::Model(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData, const std::string& textureName)
{
	this->init(vertexData, indexData, textureName);
}

void Model::init(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData, const std::string& textureName)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

	// index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

	this->vertexCount = indexData.size();

	// number of GLfloats per vertex
	static constexpr int VERTEX_ELEMENTS{ 5 };
	static constexpr int VERTEX_POS_ELEMENTS{ 3 };
	static constexpr int VERTEX_UV_ELEMENTS{ 2 };

	// vertex positional data starts on the first byte of the vertexBuffer
	constexpr int VERTEX_POS_DATA_START{ 0 * sizeof(GLfloat) };

	glVertexAttribPointer(VERTEX_POS_ATTRIB, VERTEX_POS_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_ELEMENTS * sizeof(GLfloat), (void*)VERTEX_POS_DATA_START);

	glEnableVertexAttribArray(VERTEX_POS_ATTRIB);

	// texture junk
	sf::Image textureData;
	
	if (!textureData.loadFromFile(textureName))
		textureData.create(1, 1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA, 
		textureData.getSize().x, 
		textureData.getSize().y, 
		0, 
		GL_RGBA, 
		GL_UNSIGNED_BYTE, 
		textureData.getPixelsPtr()
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	constexpr int VERTEX_UV_DATA_START{ 3 * sizeof(GLfloat) };

	glVertexAttribPointer(VERTEX_UV_ATTRIB, VERTEX_UV_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_ELEMENTS * sizeof(GLfloat), (void*)VERTEX_UV_DATA_START);

	glEnableVertexAttribArray(VERTEX_UV_ATTRIB);
}

void Model::bufferData() const
{
}