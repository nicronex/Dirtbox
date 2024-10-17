#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 lightSpacePos;

uniform sampler2D texture0;
uniform sampler2D shadowMap;
uniform vec2 shadowMapSize;
uniform mat4 lightMatrix;

float ShadowCalculation(vec4 fragPosLightSpace)
{ 
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = 0.01;
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	float shadow = 0.0;
	vec2 texelSize = 0.5 / shadowMapSize;
	for(int x = -2; x <= 2; ++x)
	{
		for(int y = -2; y <= 2; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;     
		}  
	}
	shadow /= 25.0;
	if(projCoords.z > 1.0)
		shadow = 0.0;
	return shadow;
}

void main()
{
	float shadow = ShadowCalculation(lightSpacePos);
	float lighting = 0.75 - shadow * 0.75 + 0.25;
	FragColor = vec4(texture(texture0, TexCoords).rgb * lighting, 1.0);
}
