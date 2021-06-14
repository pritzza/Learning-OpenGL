#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTextureCoords;

out vec3 pos;
out vec4 color;
out vec2 textureCoords;

uniform float posOffset;

void main()
{
	gl_Position = vec4(aPos.x, -aPos.y, aPos.z, posOffset);

	pos = aPos;
	color = aColor;
	textureCoords = aTextureCoords;
}