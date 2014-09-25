/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Simple GLSL fragment shader.
*/
#version 400

in vec3 vertex_point;

void main()
{
	gl_Position = vec3(vertex_point, 1.0);
}
