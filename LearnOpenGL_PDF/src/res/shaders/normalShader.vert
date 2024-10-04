#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT
{
	vec3 fragPos;
	vec2 texCoords;
}
vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.fragPos = (model * vec4(aPos, 1.0)).xyz;
	vs_out.texCoords = aTexCoords;
}