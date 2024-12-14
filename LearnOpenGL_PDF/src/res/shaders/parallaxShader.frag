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
	const float minLayers = 8;
	const float maxLayers = 32;
	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0, 0, 1), tangentViewDir), 0));
	float layerDepth = 1/numLayers;

	vec2 p = tangentViewDir.xy * height_scale;
	vec2 deltaTexCoords = p / numLayers;

	float currentLayerDepth = 0.0f;
	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = texture(texture_displacement, currentTexCoords).r;

	while (currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(texture_displacement, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}
	
	vec2 previousTexCoords = currentTexCoords + deltaTexCoords;
	float previousDepthMapValue = texture(texture_displacement, previousTexCoords).r;
	float previousLayerDepth = currentLayerDepth - layerDepth;

	// Get depth difference before and after collision
	float after = currentDepthMapValue - currentLayerDepth;
	float before = previousDepthMapValue - previousLayerDepth;

	// Interpolate texture coords
	float weight = after / (after - before);
	vec2 finalTexCoords = previousTexCoords * weight + currentTexCoords * (1 - weight);

	return finalTexCoords;
}

void main()
{
	// Offset texture coordinates with ParallaxMapping
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoords = ParallaxMapping(fs_in.texCoords, viewDir);
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;

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