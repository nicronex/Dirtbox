#version 330 core

out vec4 FragColor;

struct PointLight 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float strength;
};

struct DirLight 
{
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D diffuse;
uniform sampler2D specular;

uniform vec3 viewPos;

#define MATERIAL_SHININESS 32.0
#define NR_POINT_LIGHTS 2
uniform PointLight lights[NR_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 diffuseColor = texture2D(diffuse, TexCoord).rgb;
	vec3 specularColor = texture2D(specular, TexCoord).rgb;
	
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularColor.r * 32.0);
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 diffuseColor = texture2D(diffuse, TexCoord).rgb;
	vec3 specularColor = texture2D(specular, TexCoord).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularColor.r * 32.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return vec3(ambient + diffuse + specular) * light.strength;
}

void main()
{
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir); 
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(lights[i], norm, FragPos, viewDir);    
	}
    FragColor = vec4(result, 1.0);
}
