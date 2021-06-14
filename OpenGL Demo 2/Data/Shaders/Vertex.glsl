#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 passFragPosition;

uniform float posOffset;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y - 2, aPos.z, posOffset);

	passFragPosition = vec3(aPos);
}