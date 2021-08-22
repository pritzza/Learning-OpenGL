#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec3 aNormal;

out vec3 pos;
out vec2 uv;
out vec4 color;
out vec3 normal;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * transformationMatrix * vec4(aPos, 1.f);

	pos = aPos;
	uv = aUV;
	color = aColor;
	normal = aNormal;
}