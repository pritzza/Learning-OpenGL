#include "VertexArrayObject.h"

#include <glad 3.3/glad.h>

VertexArrayObject::VertexArrayObject(const unsigned int count)
	:
	m_count{ count }
{
	glGenVertexArrays(m_count, &m_id);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(m_count, &m_id);
}

void VertexArrayObject::bind()
{
	this->m_isBound = true;
	glBindVertexArray(this->m_id);
}

const unsigned int VertexArrayObject::getID() const
{
	return this->m_id;
}