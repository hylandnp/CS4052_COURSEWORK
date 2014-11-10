/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

GLSL vertex shader implementing Phong lighting model.
Based off of provided examples.
*/
#version 400

// Input attributes:
layout(location = 0) in vec3 vertex_point;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 uv_coord;


// Output attributes:
out vec2 uv_from_vertex;
out vec3 normal_from_vertex,
		 position_eye,
		 normal_eye;


// Uniform attributes:
uniform mat4 model_matrix,
			 view_matrix,
			 proj_matrix;


// Shader entry point:
void main()
{
	uv_from_vertex = uv_coord;
	normal_from_vertex = vertex_normal;

	position_eye = vec3(view_matrix * model_matrix * vec4(vertex_point, 1.0));
	normal_eye = vec3(view_matrix * model_matrix * vec4(vertex_point, 0.0));

	gl_Position = proj_matrix * vec4(position_eye, 1.0);
}
