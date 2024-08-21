in vec4 frag_color;

layout (location = 0) out vec4 out_fragColor;

void main()
{
	out_fragColor = vec4(1.0, 0.5, 0.5, 1.0);
	// out_fragColor = frag_color;
}