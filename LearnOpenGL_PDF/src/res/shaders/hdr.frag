#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse;

in vec2 texCoords;

void main()
{
	const float gamma = 2.2;

	vec3 color = texture(texture_diffuse, texCoords).rgb;

	vec3 hdrColor = color / (color + vec3(1.0));

	hdrColor = pow(hdrColor, vec3(1.0/gamma));

	FragColor = vec4(hdrColor, 1.0);
}