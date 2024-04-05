#version 330

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light
{
	// vec3 position; // no longer necessary when using directional lights.
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1);
}