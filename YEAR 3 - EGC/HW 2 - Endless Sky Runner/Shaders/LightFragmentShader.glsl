#version 330

// PhD + TODO: get color value from vertex shader
uniform sampler2D texture_1;
in vec3 color;
in vec2 texcoord;
layout(location = 0) out vec4 out_color;


void main()
{
	// PhD + TODO: write pixel out color
	vec4 color_x = texture2D(texture_1, texcoord);
	out_color = vec4(color_x);
}