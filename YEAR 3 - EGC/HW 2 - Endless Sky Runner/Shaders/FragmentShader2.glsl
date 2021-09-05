#version 330

uniform sampler2D texture_1;

in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color_x = texture2D(texture_1, texcoord);
	if (color_x.a < 0.5f) discard;
	out_color = vec4(color_x);
}