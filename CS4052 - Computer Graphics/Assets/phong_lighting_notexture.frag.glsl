/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

GLSL fragment shader implementing Phong lighting model (with Blinn-Phong specular power calculation).
Based off of provided examples.
*/
#version 400

// Input attributes:
in vec2 uv_from_vertex;
in vec3 position_eye,
		normal_eye;


// Output attributes:
out vec4 frag_colour;


// Uniform attributes:
uniform mat4 model_matrix,
			 view_matrix,
			 proj_matrix;
uniform sampler2D texture_sampler;


// Fixed point light test:
vec3 light_position_world = vec3 (5.0, 5.0, 2.0);
vec3 light_specular_colour = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 light_diffuse_colour = vec3 (0.7, 0.7, 0.7); // dull white diffuse light colour
vec3 light_ambient_colour = vec3 (0.2, 0.2, 0.2); // grey ambient colour


// Surface reflectiveness test:
vec3 surface_specular_reflect = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
vec3 surface_diffuse_reflect = vec3 (1.0, 0.5, 0.0); // orange diffuse surface reflectance
vec3 surface_ambient_reflect = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 100.0; // specular 'power'


// Shader entry point:
void main()
{
	// Diffuse intensity:
	vec3 light_position_eye = vec3(view_matrix * vec4(light_position_world, 1.0));
	vec3 distance_to_light_eye = light_position_eye - position_eye;
	vec3 direction_to_light_eye = normalize(distance_to_light_eye);
	
	vec3 diffuse_intensity = light_diffuse_colour *
							 surface_diffuse_reflect *
							 max(dot(direction_to_light_eye, normal_eye), 0.0);

	// Ambient intensity:
	vec3 ambient_intensity = light_ambient_colour * surface_ambient_reflect;

	// Specular intensity (with Blinn-Phong):
	vec3 surface_to_viewer_eye = normalize(-position_eye);
	float specular_factor = pow(max(dot(normalize(surface_to_viewer_eye + direction_to_light_eye), normal_eye), 0.0), specular_exponent);
	
	vec3 specular_intensity = light_specular_colour * surface_specular_reflect * specular_factor;

	// Output final colour:
	frag_colour = vec4(1.0) * vec4(specular_intensity + diffuse_intensity + ambient_intensity, 1.0);
}