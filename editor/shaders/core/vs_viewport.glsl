#version 410

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 1) in vec2 in_uv;

uniform mat4 uni_modelMatrix;
uniform mat4 uni_viewProjectionMatrix;

out vec3 frag_worldPosition;

void main()
{
    frag_worldPosition = (uni_modelMatrix * vec4(in_position, 1.0)).xyz;

    gl_Position = uni_viewProjectionMatrix * uni_modelMatrix * vec4(in_position, 1.0);
}