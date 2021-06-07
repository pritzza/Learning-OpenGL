#pragma once

#include "../glObjects/Color.h"

class Model;
class ShaderProgram;
class Loader;

class Renderer
{
private:
	const Color m_clearColor{ 0.2f, 0.3f, 0.3f, 1.0f };

private:
	Loader& loader;

	ShaderProgram* m_currentShaderProgram{ nullptr };

private:
	void useShaderProgram(const ShaderProgram* shaderProgram) const;

public:
	Renderer(Loader& loader);

	void draw(Model& model, const ShaderProgram* shaderProgram = nullptr) const;

	void clear() const;

	void setShaderProgram(ShaderProgram& shaderProgram);
	void unsetShaderProgram();
};