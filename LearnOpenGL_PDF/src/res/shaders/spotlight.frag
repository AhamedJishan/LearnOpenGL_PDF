#version 330

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light
{
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


out vec4 FragColor;

void main()
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	//ambient
	ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp( (theta - light.outerCutoff)/epsilon, 0.0f, 1.0f );

	//diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(lightDir, norm), 0.0f);
	diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * (distance * distance) );

	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	// Applying all the lights
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1);
}