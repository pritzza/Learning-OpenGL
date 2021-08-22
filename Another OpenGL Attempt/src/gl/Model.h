#pragma once

#include "glad/glad.h"

#include <string>
#include <vector>

// i want model to be more of a struct who's data is manipulated by others, rather than itself performing operations
struct Model
{
private:
	struct VBOConstants
	{
		const GLuint attributeIndex, size;	// size is the number of elemebts per vertex
	};

	struct VBO
	{
		VBOConstants constants;
		GLuint handle{};

		constexpr VBO(const VBOConstants& constants)
			:
			constants{ constants }
		{}
	};

private:
	static constexpr VBOConstants VERTEX_GL_DATA { 0, 3 };
	static constexpr VBOConstants UVS_GL_DATA{ 1, 2 };
	static constexpr VBOConstants COLOR_GL_DATA	 { 2, 4 };
	static constexpr VBOConstants NORMAL_GL_DATA { 3, 3 };

private:
	GLuint vao;

	// vertex buffers
	VBO verticies{ VERTEX_GL_DATA };
	VBO uvs		 { UVS_GL_DATA	  };
	VBO colors	 { COLOR_GL_DATA  };
	VBO normals	 { NORMAL_GL_DATA };

	GLuint indexBuffer;

	GLuint texture;

	GLuint vertexCount;

private:
	void bufferData(VBO& vbo, const std::vector<GLfloat>& data);
	void bufferIndices(const std::vector<GLuint>& indices);
	void bufferTexture(const std::string& textureName);

public:
	Model() = default;
	Model(
		const std::vector<GLfloat>& vertPositions,
		const std::vector<GLfloat>& vertColors,
		const std::vector<GLfloat>& vertUVs,
		const std::vector<GLuint>& indexData,
		const std::string& textureName
	);

	~Model();

	void init(
		const std::vector<GLfloat>& vertPositions,
		const std::vector<GLfloat>& vertColors,
		const std::vector<GLfloat>& vertUVs,
		const std::vector<GLuint>& indexData,
		const std::string& textureName
	);

	const GLuint getVertexCount() const  { return this->vertexCount; }
	const GLuint getTexture() const		 { return this->texture;	 }
	const GLuint getVAO() const			 { return this->vao;		 }
};