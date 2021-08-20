#include "Renderer.h"

#include "TransformationMatrix.h"
#include "ShaderProgram.h"

#include "../Entity.h"

#include <string_view>

void Renderer::render(ShaderProgram& shader, const Entity& entity) const
{
	const Model& model{ entity.getModel() };

	const TransformationMatrix transformationMatrix{ entity.getPos(), entity.getRotation(), entity.getScale() };

	static const std::string TRANSFORMATION_MATRIX_NAME{ "transformationMatrix" };

	shader.use();
	shader.setUniform(TRANSFORMATION_MATRIX_NAME, transformationMatrix);

	glBindVertexArray(model.getVAO());
	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::clear() const
{
	glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
