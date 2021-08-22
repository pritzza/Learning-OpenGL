#version 330 core

in vec3 pos;
in vec2 uv;
in vec4 color;
in vec3 normal;

out vec4 fragColor;

uniform sampler2D text;

void main()
{
    fragColor = texture(text, uv);
}