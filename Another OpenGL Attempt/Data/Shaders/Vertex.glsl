#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 UVs;

out vec3 pos;
out vec2 uvCoords;

uniform vec3 posOffset;

void main()
{
	gl_Position = vec4(aPos + (posOffset/2), 1.0) * vec4(1.0, -1.0, 1.0, 1.0);	// last bit to flip upsidedown image

	pos = aPos;
	uvCoords = UVs;
}