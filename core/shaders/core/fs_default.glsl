#version 410

in vec3 frag_worldPosition;
in vec3 frag_worldNormal;
in vec2 frag_texCoord;
in vec4 frag_color;

uniform sampler2D uni_texture0;
uniform sampler2D uni_texture1;
uniform sampler2D uni_texture2;
uniform sampler2D uni_texture3;
uniform sampler2D uni_texture4;
uniform sampler2D uni_texture5;
uniform sampler2D uni_texture6;

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
	vec4 objColor0 = texture(uni_texture0, frag_texCoord).rgba; //rbga for clarity (not needed)
	vec4 objColor1 = texture(uni_texture1, frag_texCoord);
	vec4 objColor2 = texture(uni_texture2, frag_texCoord);
	vec4 objColor3 = texture(uni_texture3, frag_texCoord);
	vec4 objColor4 = texture(uni_texture4, frag_texCoord);
	vec4 objColor5 = texture(uni_texture5, frag_texCoord);
	vec4 objColor6 = texture(uni_texture6, frag_texCoord);

	vec4 objColor = mix(objColor0, objColor1, objColor0.a); //assuming we get alpha from tex0

	out_fragColor = objColor;// * (ambient + diffuse);
}