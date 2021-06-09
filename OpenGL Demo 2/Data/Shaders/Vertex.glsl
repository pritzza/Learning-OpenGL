#version 330 core

layout (location = 0) in vec4 aPos;

out vec3 passFragPosition;

void main()
{
	gl_Position = aPos;

	passFragPosition = vec3(aPos);
}