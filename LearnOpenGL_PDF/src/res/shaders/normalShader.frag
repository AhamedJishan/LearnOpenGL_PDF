#version 330 core

out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

in VS_OUT
{
	vec3 fragPos;
	vec2 texCoords;
	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}
fs_in;

void main()
{
	vec3 baseColor = texture(texture_diffuse, fs_in.texCoords).rgb;

	// Normal Calculations------------------------------------
	vec3 normal = texture(texture_normal, fs_in.texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// Ambient lighting
	vec3 ambientColor = 0.2 * baseColor;

	// Diffuse lighting
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diffuseFactor = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = diffuseFactor * baseColor;

	// Specular Lighting
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 64);
	vec3 specularColor = specularFactor * baseColor;

	// FINAL COLOR
	FragColor = vec4((diffuseColor + specularColor), 1.0);
}