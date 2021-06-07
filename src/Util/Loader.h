#pragma once

class VertexArrayObject;
class VertexBufferObject;

class Loader
{
private:
	VertexArrayObject* m_currentlyBoundVAO{ nullptr };
	VertexBufferObject* m_currentlyBoundElementArrayBuffer{ nullptr };
	VertexBufferObject* m_currentlyBoundArrayBuffer{ nullptr };

private:
	void bindVAO(VertexArrayObject& vao);
	void bindBuffer(VertexBufferObject& vbo);

	void unbindVAO();

	void unbindElementArrayBuffer();
	void unbindArrayBuffer();
	
	void bufferIndexData(const unsigned int numInd, const unsigned int* const dataStart, unsigned int glDrawType) const;
	void bufferVertexData(const unsigned int numVert, const float* const dataStart, unsigned int glDrawType) const;

	friend class Model;
	friend class Renderer;
};