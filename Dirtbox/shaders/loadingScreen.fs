#version 330 core
in vec2 texCoord;
uniform sampler2D texture0;
uniform float progress;
out vec4 FragColor;
void main()
{
  vec3 color = texture2D(texture0, texCoord).rgb;
  vec3 grayScale = vec3(color.r + color.g + color.b) * 0.33;
  if(texCoord.x > progress)
  {
    color = grayScale;
  }
  FragColor = vec4(color, 1.0);
}
