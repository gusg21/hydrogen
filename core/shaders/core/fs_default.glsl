#version 410

in vec3 frag_worldPosition;
in vec3 frag_worldNormal;
in vec2 frag_texCoord;
in vec4 frag_color;

uniform sampler2D uni_mainTex;

layout (location = 0) out vec4 out_fragColor;

void main()
{
	float ambientAmount = 0.3f;
	float diffuseAmount = 0.7f;
	vec4 ambientColor = vec4(1, 0, 0, 1);
	vec4 sunColor = vec4(1, 0.8, 0.7, 1);
	
	vec4 ambient = ambientAmount * ambientColor;
	vec4 diffuse = diffuseAmount * sunColor * clamp(dot(frag_worldNormal, normalize(vec3(1, 1, 0))), 0, 1);

	//we might want to rework this to allow multiple textures per mesh
	vec4 objColor = texture(uni_mainTex, frag_texCoord).rgba; //rbga for clarity (not needed)

	out_fragColor = objColor;// * (ambient + diffuse);
}