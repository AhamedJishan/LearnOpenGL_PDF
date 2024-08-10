#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.05f;

uniform mat4 projection;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal * MAGNITUDE, 0.0f));
    EmitVertex();
    EndPrimitive();
}

void main() 
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
} 