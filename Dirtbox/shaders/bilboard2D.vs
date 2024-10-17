#version 330 core
layout (location = 0) in vec3 vertexPos;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 position;
uniform vec2 size;
out vec2 texCoord;
void main()
{
	vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

	vec3 vertexPosition_worldspace = 
		position
		+ cameraRight * vertexPos.x * size.x
		+ cameraUp * vertexPos.y * size.y;

	gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0f);
		
	texCoord = vertexPos.xy + vec2(0.5);
}
