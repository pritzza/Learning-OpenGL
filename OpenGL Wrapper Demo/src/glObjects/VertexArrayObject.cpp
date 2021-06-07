#include "VertexArrayObject.h"

#include <glad 3.3/glad.h>

#include "VertexBufferObject.h"

#include <iostream>

VertexArrayObject::VertexArrayObject(const unsigned int count)
	:
	m_count{ count }
{
	glGenVertexArrays(m_count, &m_id);
}

VertexArrayObject::~VertexArrayObject()
{
	if (!m_isFree)
		std::cout << "VertexArrayObject " << m_id << " memory leak\n";
}

void VertexArrayObject::free()
{
	if (!m_isFree)
	{
		glDeleteVertexArrays(m_count, &m_id);
		this->m_isFree = true;
	}
}

const unsigned int VertexArrayObject::getID() const
{
	return this->m_id;
}

bool VertexArrayObject::isValid() const
{
	return (m_isFree == false) && (m_id != 0);
}

