#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float CalculateShadow(vec4 fragPosLightSpace)
{
	vec3 ProjCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	ProjCoords = ProjCoords * 0.5 + 0.5;

	if (ProjCoords.z > 1.0)
	{
		return 0.0;
	}
	else
	{
		float currentDepthFromLight = ProjCoords.z;
		float closestDepthFromLight = texture(depthMap, ProjCoords.xy).r;
		vec3 normal = normalize(fs_in.Normal);
		vec3 lightDir = normalize(lightPos - fs_in.FragPos);
		float bias = max(0.05 *(1 - (dot(normal, lightDir))), 0.005);

		float shadow = 0.0;
		vec2 texelSize = 1.0 / textureSize(depthMap, 0);
		for(int x=-1; x<=1 ; x++)
		{
			for(int y=-1; y<=1 ; y++)
			{
				float pcfDepth = texture(depthMap, ProjCoords.xy + vec2(x, y) * texelSize).r;
				shadow += currentDepthFromLight - bias > pcfDepth ? 1.0 : 0.0;
			}
		}
		shadow /= 9.0;

		return shadow;
	}
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
	float shadow = CalculateShadow(fs_in.FragPosLightSpace);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular) ) * color;

	FragColor = vec4(lighting, 1.0);
}