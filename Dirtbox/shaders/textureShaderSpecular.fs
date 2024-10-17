 #version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 lightDiffuse;
uniform vec3 lightAmbient;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	float specular = texture2D(texture1, TexCoord).r;
	vec3 light = diff * lightDiffuse * specular + lightAmbient;
	FragColor = vec4(light * texture2D(texture0, TexCoord).rgb, 1.0);
}
