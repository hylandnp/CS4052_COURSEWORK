/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Simple GLSL fragment shader.
*/
#version 400

in vec3 colour_from_vertex;

out vec4 frag_colour;

void main()
{
	frag_colour = vec4(colour_from_vertex, 1.0);
}
