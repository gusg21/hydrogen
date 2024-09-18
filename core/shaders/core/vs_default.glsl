#version 410

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;

uniform mat4 uni_modelMatrix;
uniform mat4 uni_viewProjectionMatrix;

out vec3 frag_worldPosition;
out vec3 frag_worldNormal;
out vec2 frag_texCoord;
out vec4 frag_color;

void main()
{
	frag_texCoord = in_texCoord;

	frag_worldNormal = transpose(inverse(mat3(uni_modelMatrix))) * in_normal;
	frag_worldPosition = (uni_modelMatrix * vec4(in_position, 1.0)).xyz;

	gl_Position = uni_viewProjectionMatrix * 
				  uni_modelMatrix * 
				  vec4(in_position, 1.0);
}