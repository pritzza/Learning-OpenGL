#version 330 core

in vec3 pos;
in vec2 uvCoords;

out vec4 fragColor;

uniform vec3 posOffset;
uniform sampler2D text;

void main()
{
    fragColor = texture(text, uvCoords);
}