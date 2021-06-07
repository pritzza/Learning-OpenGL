#pragma once

class ShaderProgram;
class VertexBufferObject;

class VertexArrayObject
{
private:
	unsigned int m_id;
	const unsigned int m_count;

	bool m_isBound{ false };
	bool m_isFree{ false };

public:
	VertexArrayObject(const unsigned int count = 1);
	~VertexArrayObject();

	void free();

	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject operator= (const VertexArrayObject&) = delete;
	
	VertexArrayObject(VertexArrayObject&& vao) noexcept;

	const unsigned int getID() const;

	bool isValid() const;

	friend class Model;
	friend class Loader;
};