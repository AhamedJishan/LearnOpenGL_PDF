#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;                 // Fragment position in world space
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float farPlane;

float CalculateShadow(vec3 fragPos)
{
	vec3 lightToFrag = fragPos - lightPos;
	float closestDepth = texture(depthMap, lightToFrag).r;
	closestDepth *= farPlane;
	float currentDepth = length(lightToFrag);

	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoord).rgb;
	vec3 lightColor = vec3(0.5);
	vec3 normal = normalize(fs_in.Normal);
	
	// Ambient
	vec3 ambient = 0.3 * lightColor;

	// Diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diffuseFactor = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diffuseFactor * lightColor;

	// Specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	// Calculate shadow
	float shadow = CalculateShadow(fs_in.FragPos);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular) ) * color;

	FragColor = vec4(lighting, 1.0);
}