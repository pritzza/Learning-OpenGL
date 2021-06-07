#include "Loader.h"

#include "../glObjects/VertexArrayObject.h"
#include "../glObjects/VertexBufferObject.h"

#include <glad 3.3/glad.h>

#include <iostream>

void Loader::bindVAO(VertexArrayObject& vao)
{
	VertexArrayObject*& curVAO{ m_currentlyBoundVAO };
	
	if (curVAO != nullptr && curVAO->isValid() && curVAO->m_isBound)
	{
		this->unbindVAO();
	}
	
	curVAO = &vao;
	
	bool& isBound{ curVAO->m_isBound };
	
	if (isBound)
	{
		std::cout << "Loader: vao " << curVAO->getID() << "already bound\n";
	}
	else
	{
		isBound = true;
		glBindVertexArray(vao.getID());
	}
}

void Loader::bindBuffer(VertexBufferObject& vbo)
{
	const unsigned int newBufferObjType{ vbo.getBufferType() };
	VertexBufferObject*& curArrBuffer{ this->m_currentlyBoundArrayBuffer };	// re name this to be more generic

	if (newBufferObjType == GL_ARRAY_BUFFER)
	{
		if (curArrBuffer != nullptr && curArrBuffer->isValid() && curArrBuffer->m_isBound)
		{
			this->unbindVAO();
		}
	}
	else if (newBufferObjType == GL_ELEMENT_ARRAY_BUFFER)
	{
		if (m_currentlyBoundElementArrayBuffer != nullptr && m_currentlyBoundVAO->isValid() && m_currentlyBoundVAO->m_isBound)
		{
			this->unbindVAO();
		}
	}

	VertexBufferObject& newArrBuffer = vbo;

	bool& isBound{ newArrBuffer.m_isBound };

	if (isBound)
	{
		std::cout << "Loader: vao " << newArrBuffer.getID() << "already bound\n";
	}
	else
	{
		isBound = true;
		glBindBuffer(newArrBuffer.getBufferType(), newArrBuffer.getID());

		if (newBufferObjType == GL_ELEMENT_ARRAY_BUFFER)
			this->m_currentlyBoundElementArrayBuffer = &newArrBuffer;
		else if (newBufferObjType == GL_ARRAY_BUFFER)
			this->m_currentlyBoundArrayBuffer = &newArrBuffer;
	}
}

void Loader::unbindVAO()
{
	bool& isBound{ this->m_currentlyBoundVAO->m_isBound };
	
	if (isBound)
	{
		isBound = false;
		glBindVertexArray(0);
	}
	else
	{
		std::cout << "Loader: vao " << this->m_currentlyBoundVAO->getID() << " already unbound\n";
	}
}

void Loader::unbindElementArrayBuffer()
{
	VertexBufferObject*& eab{ this->m_currentlyBoundElementArrayBuffer };
	
	if (eab != nullptr && eab->isValid())
	{
		//glBindBuffer(eab->getBufferType(), 0);	// dont actually unbind i guess
	
		eab->m_isBound = false;
		eab = nullptr;
	}
	else
	{
		std::cout << "Loader: trying to unbind element array buffer vbo to non existant or invalid VAO\n";
	}
}

void Loader::unbindArrayBuffer()
{
	VertexBufferObject*& ab{ this->m_currentlyBoundArrayBuffer };
	
	if (ab != nullptr && ab->isValid() && ab->m_isBound)
	{
		glBindBuffer(ab->getBufferType(), 0);
	
		ab->m_isBound = false;
		ab = nullptr;
	}
	else
	{
		std::cout << "Loader: trying to unbind array buffer vbo to non existant or invalid VAO\n";
	}
}

void Loader::bufferIndexData(const unsigned int dataSize, const unsigned int* const dataStart, unsigned int glDrawType) const
{
	this->m_currentlyBoundElementArrayBuffer->setSize(dataSize);
	this->m_currentlyBoundElementArrayBuffer->setNumElements(dataSize / sizeof(unsigned int));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, dataStart, glDrawType);
}

void Loader::bufferVertexData(const unsigned int dataSize, const float* const dataStart, unsigned int glDrawType) const
{
	this->m_currentlyBoundArrayBuffer->setSize(dataSize);
	this->m_currentlyBoundArrayBuffer->setNumElements(dataSize / sizeof(float));

	glBufferData(GL_ARRAY_BUFFER, dataSize, dataStart, glDrawType);
}
