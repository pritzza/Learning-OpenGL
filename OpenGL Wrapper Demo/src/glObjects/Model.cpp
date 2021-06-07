#include "Model.h"

#include <glad 3.3/glad.h>

#include "../Util/Loader.h"

#include "../Util/GLDebugCallBack.h"

Model::Model(Loader& loader)
	:
	m_vao{},
	m_vbos{}	// will with BufferSize 0s
{
	loader.bindVAO(m_vao);

	const int v = static_cast<int>(BufferIndex::Vertices);
	m_vbos[v].setBufferType(GL_ARRAY_BUFFER);

	const int i = static_cast<int>(BufferIndex::Indices);
	m_vbos[i].setBufferType(GL_ELEMENT_ARRAY_BUFFER);
	checkError("Model::Model");
}

Model::~Model()
{
	this->m_vao.free();

	for (VertexBufferObject& vbo : this->m_vbos)
		vbo.free();
}

void Model::setVertices(Loader& loader, const std::vector<Vertex>& vertices)
{
	// to be implemented
}

void Model::setVertices(Loader& loader, const std::vector<float>& vertices)
{
	const int vIndex{ static_cast<int> (BufferIndex::Vertices) };

	loader.bindVAO(m_vao);
	loader.bindBuffer(m_vbos[vIndex]);
	checkError("Model::setVert::bindBuffer");

	loader.bufferVertexData(vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	checkError("Model::setVert::bufferVertData");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	checkError("Model::setVert::VertAttribPtr");

	loader.unbindVAO();
	loader.unbindArrayBuffer();
	checkError("Model::setVert::unbinds");
}

void Model::setIndices(Loader& loader, const std::vector<unsigned int>& indices)
{
	const int iIndex{ static_cast<int> (BufferIndex::Indices) };

	loader.bindVAO(m_vao);
	loader.bindBuffer(m_vbos[iIndex]);
	checkError("Model::setInd::bindEBO");

	loader.bufferIndexData(indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	checkError("Model::setInd::bufferEBO");

	loader.unbindVAO();
	loader.unbindElementArrayBuffer();
	checkError("Model::setInd::unbind");
}

const VertexArrayObject& Model::getVAO() const
{
	return this->m_vao;
}

VertexArrayObject& Model::getVAO()
{
	return this->m_vao;
}

VertexBufferObject& Model::getVBO()
{
	const int vIndex{ static_cast<int> (BufferIndex::Vertices) };

	return this->m_vbos[vIndex];
}

const unsigned int Model::getBufferID() const
{
	const int vIndex{ static_cast<int> (BufferIndex::Vertices) };

	return this->m_vbos[vIndex].getID();
}

const unsigned int Model::getIndices() const
{
	const int iIndex{ static_cast<int> (BufferIndex::Indices) };

	return this->m_vbos[iIndex].getID();
}

const unsigned int Model::getNumIndices() const
{
	const int iIndex{ static_cast<int> (BufferIndex::Indices) };

	return this->m_vbos[iIndex].m_numElements;
}
