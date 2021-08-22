#include "Model.h"

#include <SFML/Graphics/Image.hpp>

#include <iostream>

#include "../util/FileLoader.h"

Model::Model(const std::vector<GLfloat>& vertPositions,
	const std::vector<GLfloat>& vertColors,
	const std::vector<GLfloat>& vertUVs,
	const std::vector<GLuint>& indexData,
	const std::string& textureName)
{
	this->init(vertPositions, vertColors, vertUVs, indexData, textureName);
}

Model::~Model()
{
	glDeleteTextures(1, &texture);

	glDeleteBuffers(1, &indexBuffer);

	glDeleteBuffers(1, &normals.handle);
	glDeleteBuffers(1, &colors.handle);
	glDeleteBuffers(1, &uvs.handle);
	glDeleteBuffers(1, &verticies.handle);

	glDeleteVertexArrays(1, &vao);
}

void Model::init(
	const std::vector<GLfloat>& vertexData,
	const std::vector<GLfloat>& colorData,
	const std::vector<GLfloat>& uvData,
	const std::vector<GLuint>& indexData,
	const std::string& textureName)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	this->bufferData(this->verticies, vertexData);
	this->bufferData(this->colors,	  colorData );
	this->bufferData(this->uvs,		  uvData	);
	//this->bufferData(this->normals, normalData);

	this->bufferIndices(indexData);
	this->bufferTexture(textureName);
}

void Model::bufferData(VBO& vbo, const std::vector<GLfloat>& data)
{
	// vertex buffer
	glGenBuffers(1, &vbo.handle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.handle);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(
		vbo.constants.attributeIndex,			// index
		vbo.constants.size,						// size
		GL_FLOAT,								// type
		GL_FALSE,								// normalized
		vbo.constants.size * sizeof(GLfloat),	// stride
		(void*)0								// pointer/offset
	);

	glEnableVertexAttribArray(vbo.constants.attributeIndex);
}

void Model::bufferIndices(const std::vector<GLuint>& indices)
{
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	this->vertexCount = indices.size();
}

void Model::bufferTexture(const std::string& textureName)
{
	const FileLoader fl;
	const sf::Image textureData{ fl.getImage(textureName) };

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
}