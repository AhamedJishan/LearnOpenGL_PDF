#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sampler;

void main()
{
	vec4 texColor = texture(sampler, TexCoords);
	if (texColor.a < 0.1)
		discard;
	FragColor = texColor;
}