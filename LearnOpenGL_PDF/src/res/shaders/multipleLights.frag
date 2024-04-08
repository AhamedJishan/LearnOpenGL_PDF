#version 330

// Inputs from Vertex shader
// -------------------------
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Shader output
// -------------
out vec4 FragColor;

// Structs
// -------
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
struct SpotLight
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

#define NR_POINT_LIGHTS 4

// Uniforms
// --------
uniform Material material;

uniform DirectionalLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 viewPos;

// Function definitions
// --------------------
vec3 CalculateDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLighting(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	result = CalculateDirectionalLighting(dirLight, norm, viewDir);
	result += CalculateSpotLighting(spotLight, norm, FragPos,  viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalculatePointLighting(pointLight[i], norm, FragPos, viewDir);

	// Applying all the lights
	FragColor = vec4(result, 1);
}

vec3 CalculateDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(lightDir, normal), 0.0);
	// specular shading
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 32.0f);

	// Combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);

	// diffuse shading
	float diff = max(dot(lightDir, normal), 0.0);
	// specular shading
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance);

	// Combine results
	vec3 diffuse = light.diffuse * attenuation * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * attenuation * spec * vec3(texture(material.specular, TexCoords));

	return (diffuse + specular);
}

vec3 CalculateSpotLighting(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.direction);
	vec3 fragDir = normalize(fragPos - light.position);

	// Intensity: 1 when inside innerCutoff; 0 when outside outcutoff, and 0-1 when between inner and outer cutoff
	float theta = dot(lightDir, fragDir);
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

	// diffuse shading
	float diff = max(dot(lightDir, fragDir), 0.0f);

	// specular shading
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float spec = pow(dot(reflectDir, viewDir), material.shininess);

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance);

	// Combine results
	vec3 diffuse = light.diffuse * diff * attenuation * intensity * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * attenuation * intensity * vec3(texture(material.specular, TexCoords));

	return (diffuse + specular);
}