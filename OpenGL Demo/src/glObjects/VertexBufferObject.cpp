#include "VertexBufferObject.h"

#include <glad 3.3/glad.h>

#include <iostream>

VertexBufferObject::VertexBufferObject(const unsigned int count)
	:
	m_count{count},
	m_bufferType{ 0 },
	m_numElements{ 0 },
	m_size{ 0 }
{
	glGenBuffers(m_count, &m_id);
}

VertexBufferObject::~VertexBufferObject()
{
	if (!m_isFree)
		std::cout << "VertexBufferObject " << m_id << " memory leak\n";
}

void VertexBufferObject::free()
{
	if (!m_isFree)
	{
		glDeleteBuffers(m_count, &m_id);
		this->m_isFree = true;
	}
}

//VertexBufferObject::VertexBufferObject(VertexBufferObject&& vao)
//	:
//	m_id{ vao.m_id },
//	m_count{ vao.m_count },
//	m_bufferType{ vao.getBufferType() },
//	m_isBound{ m_isBound }
//{
//	this->m_id = 0;	// set to null id
//	this->m_isBound = false;
//	this->m_bufferType = 0;
//}

void VertexBufferObject::setNumElements(const unsigned int numElements)
{
	this->m_numElements = numElements;
}

void VertexBufferObject::setSize(const unsigned int size)
{
	this->m_size = size;
}

void VertexBufferObject::setBufferType(const unsigned int bt)
{
	this->m_bufferType = bt;
}

const unsigned int VertexBufferObject::getID() const
{
	return this->m_id;
}

const unsigned int VertexBufferObject::getBufferType() const
{
	return this->m_bufferType;
}

const unsigned int VertexBufferObject::getNumElements() const
{
	return this->m_numElements;
}

bool VertexBufferObject::isValid() const
{
	return (m_isFree == false) && (m_id != 0);
}
