#version 330 core

out vec4 FragColor;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_displacement;

uniform float height_scale;

in VS_OUT
{
	vec3 fragPos;
	vec2 texCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}
fs_in;

vec2 ParallaxMapping (vec2 texCoords, vec3 tangentViewDir)
{
	float height = texture(texture_displacement, texCoords).r;
	vec2 p = tangentViewDir.xy / tangentViewDir.z * (height * height_scale);
	return texCoords - p;
}

void main()
{
	// Offset texture coordinates with ParallaxMapping
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoords = ParallaxMapping(fs_in.texCoords, viewDir);

	// Obtain normal from normal map
	vec3 normal = texture(texture_normal, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 baseColor = texture(texture_diffuse, texCoords).rgb;

	// Ambient lighting
	vec3 ambientColor = 0.1 * baseColor;

	// Diffuse lighting
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuseColor = diff * baseColor;

	// Specular Lighting
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specularColor = specularFactor * vec3(0.2);

	// FINAL COLOR
	FragColor = vec4((ambientColor + diffuseColor + specularColor), 1.0);
	//FragColor = vec4(baseColor, 1.0);
}