#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;
out vec2 texcoord;

void main()
{
	// PhD + TODO: compute world space vectors
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
	vec3 world_normal = normalize(mat3(Model) * v_normal);
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);

	// PhD + TODO: define ambient light component
	float ambient_light = material_kd * 0.85;

	// PhD + TODO: compute diffuse light component
	float diffuse_light = material_kd * max(dot(normalize(world_normal), L), 0);

	// PhD + TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		//specular_light = material_ks * pow(max(dot(V, L), 0), material_shininess);
		specular_light = material_ks * pow(max(dot(normalize(world_normal), H), 0), material_shininess);
	}

	// PhD + TODO: compute light
	float light_intensity = ambient_light + (1 / (pow(distance(v_position, light_position),2) + 1)) * (diffuse_light + specular_light);

	// PhD + TODO: send color light output to fragment shader
	color = object_color * light_intensity;
	texcoord = v_texture_coord * light_intensity;
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
