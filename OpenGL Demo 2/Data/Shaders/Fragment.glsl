#version 330 core

in vec3 pos;
in vec4 color;
in vec2 textureCoords;

out vec4 fragColor;

//uniform float colorOffset;
uniform sampler2D texture;

void main()
{
    fragColor = texture(texture, textureCoords);
}