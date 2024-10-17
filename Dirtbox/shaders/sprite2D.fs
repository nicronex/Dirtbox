#version 330 core
in vec2 texCoord;
uniform sampler2D texture0;
out vec4 FragColor;
void main()
{
   vec4 color = texture2D(texture0, texCoord);
   if(color.a == 0.0)
   	discard;
   FragColor = color;
}
