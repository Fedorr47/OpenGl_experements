#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float xOffset;
uniform float zOffset;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position.x + xOffset, 
	position.y, 
	position.z + zOffset,
	1.0f);
    ourColor = color;
	TexCoord = vec2(texCoord.x, 1 - texCoord.y);
}