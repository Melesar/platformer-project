#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

uniform mat3 transform;

out vec2 texCoord0;

void main()
{
    texCoord0 = texCoord;
	texCoord0.y = 1 - texCoord0.y;
	vec3 pos = transform * vec3(position, 1);
	gl_Position = vec4(pos.xy, 0, 1.0);
}