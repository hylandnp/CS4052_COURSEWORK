/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Simple GLSL vertex shader.
*/
#version 400

layout(location = 0) in vec3 vertex_point;
layout(location = 1) in vec3 vertex_colour;
layout(location = 2) uniform mat4 transformation;

out vec3 colour_from_vertex;

void main()
{
	colour_from_vertex = vertex_colour;
	gl_Position = transformation * vec4(vertex_point, 1.0);
}
