#version 300 es

precision mediump float;

in vec3 frag_worldPosition;
in vec3 frag_worldNormal;
in vec4 frag_color;

layout (location = 0) out vec4 out_fragColor;

void main()
{
	float ambientAmount = 0.3f;
	float diffuseAmount = 0.7f;
	vec4 ambientColor = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 sunColor = vec4(1.0, 0.8, 0.7, 1.0);
	
	vec4 ambient = ambientAmount * ambientColor;
	vec4 diffuse = diffuseAmount * sunColor * clamp(dot(frag_worldNormal, normalize(vec3(1.0, 1.0, 0.0))), 0.0, 1.0);
	
//	out_fragColor = ambient + diffuse;
	out_fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
