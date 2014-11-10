/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

GLSL vertex shader.
*/
#version 400

layout(location = 0) in vec3 vertex_point;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 uv_coord;
out vec2 uv_from_vertex;

uniform mat4 mvp_matrix;

// Shader entry point:
void main()
{
	uv_from_vertex = uv_coord;
	gl_Position = mvp_matrix * vec4(vertex_point, 1.0);
}
