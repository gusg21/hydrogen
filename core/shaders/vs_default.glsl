#version 460

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;

out vec4 frag_color;

void main()
{
	frag_color = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = vec4(in_position, 1.0);
}