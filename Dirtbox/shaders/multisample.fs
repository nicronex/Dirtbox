#version 330 core
in vec2 texCoord;
uniform sampler2DMS texture0;
out vec4 FragColor;
void main()
{
   vec3 color = texelFetch(texture0, ivec2(texCoord.x, texCoord.y), 3).rgb;
   FragColor = vec4(color, 1.0);
}
