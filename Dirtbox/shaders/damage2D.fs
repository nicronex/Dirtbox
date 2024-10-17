#version 330 core
in vec2 texCoord;
uniform sampler2D texture0;
out vec4 FragColor;
uniform float anim;
void main()
{
   vec4 color = texture2D(texture0, texCoord);
   color.g *= 1.0 - anim;
   color.b *= 1.0 - anim;
   if(color.a == 0.0)
   	discard;
   FragColor = color;
}
