#version 330 core
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 texPos;
uniform mat4 lightMatrix;
uniform mat4 model;
void main()
{
	gl_Position = lightMatrix * model * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
}
