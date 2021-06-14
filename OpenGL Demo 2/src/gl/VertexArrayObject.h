#pragma once

#include <cstdint>
#include <vector>

struct BasicVertex;
struct ColoredVertex;
struct TexturedVertex;
struct ColoredTexturedVertex;

class VertexArrayObject final
{
private:
	uint32_t vao;	// VertexArrayObject
	uint32_t vbo;	// VertexBufferObject (vertices)
	uint32_t ibo;	// VertexBufferObject (indices)

	uint32_t numIndices{};

private:
	void bufferVertexData(const std::vector<float>& vertices) const;
	void bufferVertexData(const std::vector<BasicVertex>& vertices) const;
	void bufferVertexData(const std::vector<ColoredVertex>& vertices) const;
	void bufferVertexData(const std::vector<TexturedVertex>& vertices) const;
	void bufferVertexData(const std::vector<ColoredTexturedVertex>& vertices) const;

	void bufferIndicesData(const std::vector<uint32_t>& indices);

public:
	VertexArrayObject();
	~VertexArrayObject();

	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(VertexArrayObject&&);

	VertexArrayObject(VertexArrayObject&&) noexcept;

	template <typename VertexType>
	void makeVAO(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices);

	//void makeVBO(const std::string& fileName);	// load model from file

	void draw();

	void bind() const;
	void unbind() const;
};

template <typename VertexType>
void VertexArrayObject::makeVAO(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices)
{
	this->bind();

	bufferVertexData(vertices);
	bufferIndicesData(indices);

	this->unbind();
}