#include "VertexBufferObject.h"

#include <glad 3.3/glad.h>

VertexBufferObject::VertexBufferObject(const unsigned int count)
	:
	m_count{count}
{
	glGenBuffers(m_count, &m_id);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(m_count, &m_id);
}

void VertexBufferObject::bind(const unsigned int bufferType)
{
	this->m_bufferType = bufferType;
	this->m_isBound = true;
	glBindBuffer(bufferType, this->m_id);
}

const unsigned int VertexBufferObject::getID() const
{
	return this->m_id;
}

const unsigned int VertexBufferObject::getBufferType() const
{
	return this->m_bufferType;
}
