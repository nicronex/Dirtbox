 #version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPos;
uniform vec3 lightDiffuse;
uniform vec3 lightAmbient;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 light = diff * lightDiffuse + lightAmbient;
	vec3 color0 = texture2D(texture0, TexCoord).rgb;
	vec3 color1 = texture2D(texture1, vec2(-TexCoord.y, TexCoord.x) * 1.1).rgb;
	vec3 color2 = texture2D(texture2, vec2(TexCoord.y, -TexCoord.x) * 1.3).rgb;
	FragColor = vec4(light * mix(color2, mix(color0, color1, sin(TexCoord.x * 0.15 + TexCoord.y * 0.1)), cos(TexCoord.x * 0.13- TexCoord.y * 0.18)), 1.0);
}
