#include "Renderer.h"

#include "../math/MatrixMaker.h"
#include "../gl/ShaderProgram.h"

#include "../Entity.h"

#include <string_view>

Renderer::Renderer(const int width, const int height)
	:
	width{ width },
	height{ height }
{
	MatrixMaker mm;
	this->projectionMatrix = mm.makeProjectionMatrix(width, height, fov, near, far);
}

void Renderer::render(ShaderProgram& shader, const Entity& entity) const
{
	const Model& model{ entity.getModel() };

	const MatrixMaker mm;

	const glm::mat4 transformationMatrix{ mm.makeTransformationMatrix(entity.getPos(), entity.getRotation(), entity.getScale()) };
	const glm::mat4 viewMatrix{ mm.makeViewMatrix(this->camera.getPos(), this->camera.getRotation()) };

	static const std::string TRANSFORMATION_MATRIX_NAME{ "transformationMatrix" };
	static const std::string PROJECTION_MATRIX_NAME{ "projectionMatrix" };
	static const std::string VIEW_MATRIX_NAME{ "viewMatrix" };

	shader.use();
	shader.setUniform(TRANSFORMATION_MATRIX_NAME, transformationMatrix);
	shader.setUniform(PROJECTION_MATRIX_NAME, projectionMatrix);
	shader.setUniform(VIEW_MATRIX_NAME, viewMatrix);

	glBindVertexArray(model.getVAO());
	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::clear() const
{
	glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}