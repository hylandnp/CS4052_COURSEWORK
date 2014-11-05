/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

GLSL fragment shader.
*/
#version 400

in vec2 uv_from_vertex;
out vec4 frag_colour;

uniform sampler2D texture_sampler;


void main()
{
	frag_colour = texture2D(texture_sampler, uv_from_vertex);
}
