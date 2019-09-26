#version 330 core

in vec2 texCoord0;

uniform sampler2D diffuse;
uniform vec4 color;

out vec4 FragColor;

void main()
{
    //FragColor = vec4(0.97, 0.57, 0.12, 1);
    FragColor = texture(diffuse, texCoord0) * color;
    //FragColor = color;
} 