#version 330 core

in vec3 pos;
in vec4 color;

out vec4 fragColor;

uniform float colorOffset;

void main()
{
    fragColor = vec4(color.rgb + colorOffset/10.f, color.a); 
}