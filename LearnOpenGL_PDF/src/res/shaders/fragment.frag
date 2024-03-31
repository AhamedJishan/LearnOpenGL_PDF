#version 330

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 result = (diffuse + ambient) * objectColor;

	FragColor = vec4(result, 1);
}