#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 outlineColor;

out vec3 color;

void main()
{
	color = outlineColor;
	gl_Position = projection * view * model * vec4(vertex, 1.0);
}
