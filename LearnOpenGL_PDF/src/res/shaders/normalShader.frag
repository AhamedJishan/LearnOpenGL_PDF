#version 330 core

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

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
	vec3 baseColor = texture(texture_diffuse1, fs_in.texCoords).rgb;

	// Normal Calculations------------------------------------
	vec3 normal = texture(texture_normal1, fs_in.texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	// Ambient lighting
	vec3 ambientColor = 0.1 * baseColor;

	// Diffuse lighting
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuseColor = diff * baseColor;

	// Specular Lighting
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specularColor = specularFactor * texture(texture_specular1, fs_in.texCoords).rgb;

	// FINAL COLOR
	FragColor = vec4((ambientColor + diffuseColor + specularColor), 1.0);
	//FragColor = vec4(baseColor, 1.0);
}