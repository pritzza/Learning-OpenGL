#version 330 core

// attribute positions based on vertex attribute classes in src/Application.cpp
layout(location = 0) in vec3 aPos;
layout(location = 3) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

// for diffuse lighting
out vec3 normal;
out vec3 fragPosition;
out vec2 texCoord;

// for camera transform
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	// for diffuse lighting
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	normal = normalize(normalMatrix * aNormal);

	fragPosition = vec3(model * vec4(aPos, 1.0));

	texCoord = aTexCoord;

	gl_Position = perspective * view * model * vec4(aPos, 1.0);
}