#pragma once

class VertexBufferObject
{
private:
	unsigned int m_id;
	const unsigned int m_count;

	unsigned int m_bufferType;
	bool m_isBound{ false };
	bool m_isFree{ false };

	unsigned int m_numElements;
	unsigned int m_size;

	// would probably also be useful to know the actual data type and data contents of what each vbo

public:
	VertexBufferObject(const unsigned int count = 1);
	~VertexBufferObject();

	void free();

	VertexBufferObject(const VertexBufferObject&) = delete;
	VertexBufferObject operator= (const VertexBufferObject&) = delete;
	
	VertexBufferObject(VertexBufferObject&& vao) noexcept;

	void setNumElements(const unsigned int numElements);
	void setSize(const unsigned int size);
	void setBufferType(const unsigned int bt);

	const unsigned int getID() const;
	const unsigned int getBufferType() const;
	const unsigned int getNumElements() const;

	bool isValid() const;

	friend class Model;
	friend class Loader;
};