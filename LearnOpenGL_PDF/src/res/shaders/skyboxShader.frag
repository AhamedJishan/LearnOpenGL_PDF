#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube sampler;

void main()
{
	FragColor = texture(sampler, TexCoords);
}