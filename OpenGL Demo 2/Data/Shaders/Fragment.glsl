#version 330 core

in vec3 passFragPosition;

out vec4 fragColor;

uniform float colorOffset;

void main()
{
    fragColor = vec4(passFragPosition.xyz, 1.0); 
}