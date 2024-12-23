#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

struct Light {
	vec3 Position;
	vec3 Color;
};

uniform Light[16] lights;
uniform vec3 viewPos;

void main()
{
	// No ambient and specular, only diffuse this time

	vec3 color = texture(texture_diffuse, fs_in.TexCoords).rgb;
	
	vec3 fragPos = fs_in.FragPos;
	vec3 normal = normalize(fs_in.Normal);

	vec3 lighting = vec3(0.0f);

	for (int i = 0; i < 16; i++)
	{
		vec3 lightDir = normalize(lights[i].Position - fragPos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = lights[i].Color * diff * color;

		vec3 result = diffuse;

		// Attenuation
		float distance = length(fragPos - lights[i].Position);
		result *= 1.0 / (distance * distance);

		lighting += result;
	}

	FragColor = vec4(lighting, 1.0);
}