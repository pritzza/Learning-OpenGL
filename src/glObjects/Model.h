#pragma once

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Vertex.h"

#include <array>
#include <vector>

class Loader;

enum class BufferIndex
{
	Vertices,
	Indices,
	// normals
	//texture map
	// whatever crazy stuff later
	BufferSize
};

class Model
{
private:
	VertexArrayObject m_vao;
	std::array<VertexBufferObject, static_cast<int>(BufferIndex::BufferSize)> m_vbos;
	
public:
	Model(Loader& loader);	// allocates 1 vao and BufferSize vbos on gpu
	~Model();

	void setVertices(Loader& loader, const std::vector<Vertex>& vertices);
	void setVertices(Loader& loader, const std::vector<float>& vertices);
	void setIndices (Loader& loader, const std::vector<unsigned int>& indexes);

	const VertexArrayObject& getVAO() const;
	VertexArrayObject& getVAO();

	VertexBufferObject& getVBO();

	const unsigned int getBufferID() const;

	const unsigned int getIndices() const;
	const unsigned int getNumIndices() const;
};