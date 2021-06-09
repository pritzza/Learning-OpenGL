#pragma once

#include <cstdint>
#include <vector>

class VertexArrayObject
{
private:
	uint32_t vao;	// VertexArrayObject
	uint32_t vbo;	// VertexBufferObject (vertices)
	uint32_t ibo;	// VertexBufferObject (indices)

	uint32_t numIndices{};

private:
	void bufferVertexData(const std::vector<float>& vertices) const;
	void bufferIndicesData(const std::vector<uint32_t>& indices);

public:
	VertexArrayObject();
	~VertexArrayObject();

	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(VertexArrayObject&&);

	VertexArrayObject(VertexArrayObject&&) noexcept;

	void makeVAO(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
	//void makeVBO(const std::string& fileName);	// load model from file

	void draw();

	void bind() const;
	void unbind() const;
};