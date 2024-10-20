#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT
{
	vec3 fragPos;
	vec2 texCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}
vs_out;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.fragPos = (model * vec4(aPos, 1.0)).xyz;
	vs_out.texCoords = aTexCoords;

	mat3 normalMat = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMat * aTangent);
	vec3 B = normalize(normalMat * aBitangent);
	vec3 N = normalize(normalMat * aNormal);
	// for orthogonal matrices transpose is the same is inverse
	mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vec3(model * vec4(aPos, 1.0));
}