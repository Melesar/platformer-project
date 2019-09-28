#version 330 core

in vec2 texCoord0;

uniform sampler2D diffuse;
uniform vec4 color;

out vec4 FragColor;

void main()
{
    FragColor = texture(diffuse, texCoord0) * color;
} 