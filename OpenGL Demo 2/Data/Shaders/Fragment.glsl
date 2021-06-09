#version 330 core

in vec3 passFragPosition;

out vec4 FragColor;

void main()
{
    FragColor = vec4(passFragPosition.xyz + 0.5, 1.0); 
}