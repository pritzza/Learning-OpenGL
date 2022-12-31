#pragma once

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	Texture(const std::string_view& path, GLuint targetTexture);

	// delete copy, move and assignment constructor to prevent any chance of
	// unintentional memory leaks
	Texture(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator= (const Texture&) = delete;

	~Texture();

	// configure wrapping and filter behavior
	void configure(
		GLuint horizontalWrap,
		GLuint verticalWrap,
		GLuint minFilter,
		GLuint magFilter
	);

	// apply this texture to a given texture unit
	void apply(GLuint textureUnit);

	void bind() const;

	GLuint getHandle() const { return handle; }

private:
	GLuint handle;
	const GLuint targetTexture;

private:
	void bufferData(
		int width, 
		int height, 
		int numChannels, 
		const unsigned char* const data
	);
};