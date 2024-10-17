#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
	Normal = normal;
	TexCoord = uv;
	FragPos = vec3(model * vec4(vertex, 1.0));
	gl_Position = projection * view * model * vec4(vertex, 1.0);
}
