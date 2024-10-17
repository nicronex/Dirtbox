#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 texPos;

out vec2 TexCoords;
out vec4 lightSpacePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightMatrix;

void main()
{
	TexCoords = texPos;
	lightSpacePos = lightMatrix * model * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);

	gl_Position = projection * view * model * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
}
