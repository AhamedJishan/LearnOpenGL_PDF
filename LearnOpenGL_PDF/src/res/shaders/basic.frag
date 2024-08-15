#version 330 core

out vec4 FragColor;
uniform sampler2D sampler;
  
in vec2 texCoords;

void main()
{
    FragColor = texture(sampler, texCoords);
}