#pragma once

class VertexBufferObject
{
private:
	unsigned int m_id;
	const unsigned int m_count;

	unsigned int m_bufferType;
	bool m_isBound{ false };

public:
	VertexBufferObject(const unsigned int count);
	~VertexBufferObject();

	void bind(const unsigned int bufferType);

	const unsigned int getID() const;
	const unsigned int getBufferType() const;

};