#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sampler;

void main()
{
	FragColor = vec4(vec3(1 - texture(sampler, TexCoords)), 1.0f);
}