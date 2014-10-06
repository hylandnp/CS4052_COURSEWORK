/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Simple GLSL fragment shader.
*/
#version 400

layout(location = 0) in vec3 vertex_point;
layout(location = 1) in vec3 vertex_colour;

out vec3 colour_from_vertex;

void main()
{
	colour_from_vertex = vertex_colour;
	gl_Position = vec4(vertex_point, 1.0);
}
