#version 410

in vec3 frag_worldPosition;
in vec4 frag_color;

layout (location = 0) out vec4 out_fragColor;

void main()
{
    out_fragColor = vec4(1, 0, 0, 1);
}