#include "Model.h"

Model::Model(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData)
{
	this->init(vertexData, indexData);
}

void Model::init(const std::vector<GLfloat>& vertexData, const std::vector<GLuint>& indexData)
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
	static constexpr int VERTEX_ELEMENTS{ 3 };

	// vertex positional data starts on the first byte of the vertexBuffer
	constexpr void* VERTEX_POS_DATA_START{ 0 };

	glVertexAttribPointer(0, VERTEX_ELEMENTS, GL_FLOAT, GL_FALSE, VERTEX_ELEMENTS * sizeof(GLfloat), VERTEX_POS_DATA_START);

	glEnableVertexAttribArray(0);
}

void Model::bufferData() const
{
}