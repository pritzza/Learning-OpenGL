#pragma once

class VertexArrayObject
{
private:
	unsigned int m_id;
	const unsigned int m_count;
	bool m_isBound{ false };

public:
	VertexArrayObject(const unsigned int count);
	~VertexArrayObject();

	void bind();

	const unsigned int getID() const;
};