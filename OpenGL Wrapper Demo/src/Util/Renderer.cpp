#include "Renderer.h"

#include "../glObjects/Model.h"
#include "../glObjects/VertexArrayObject.h"
#include "../glShaders/ShaderProgram.h"
#include "Loader.h"

//#include "../Util/GLDebugCallBack.h"

#include <glad 3.3/glad.h>

#include <iostream>

Renderer::Renderer(Loader& loader)
	:
	loader{ loader }
{
}

void Renderer::draw(Model& model, const ShaderProgram* shaderProgram) const
{
	this->useShaderProgram(shaderProgram);
	//checkError_("Renderer::draw::userShader");

	loader.bindVAO( model.getVAO() );
	//checkError_("Renderer::draw::bindVAO");

	glDrawElements(GL_TRIANGLES, model.getNumIndices(), GL_UNSIGNED_INT, (void*) model.getIndices());
	//checkError_("Renderer::draw::drawEle");

	this->useShaderProgram(this->m_currentShaderProgram);
	
	loader.unbindVAO();
	//checkError_("Renderer::draw::unbind");

}

void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(
		this->m_clearColor.r, 
		this->m_clearColor.g,
		this->m_clearColor.b, 
		this->m_clearColor.a
	);
}

void Renderer::useShaderProgram(const ShaderProgram* shaderProgram) const
{
	if (shaderProgram != nullptr)
		glUseProgram(shaderProgram->getID());
	else if (this->m_currentShaderProgram != nullptr)
		glUseProgram(this->m_currentShaderProgram->getID());
	else
		std::cout << "No valid Shader Program\n";
}

void Renderer::setShaderProgram(ShaderProgram& shaderProgram)
{
	this->m_currentShaderProgram = &shaderProgram;
}

void Renderer::unsetShaderProgram()
{
	this->m_currentShaderProgram = nullptr;
}
