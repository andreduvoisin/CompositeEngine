#version 330 core

// Input color from the vertex program.
in vec4 v2f_color;

layout (location=0) out vec4 out_color;

void main()
{
    out_color = v2f_color;
}