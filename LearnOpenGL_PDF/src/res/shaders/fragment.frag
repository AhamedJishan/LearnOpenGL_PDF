#version 330

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D myTexture;

void main()
{
	FragColor = texture(myTexture, TexCoord);
}