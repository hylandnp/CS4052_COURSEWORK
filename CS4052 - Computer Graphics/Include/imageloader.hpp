/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Test image/texture loading function.
Adapted from FreeImage examples.
*/
#pragma once
#include "OpenGL.hpp"

#include <FreeImage.h>
#include <FreeImageIO.h>
#include <string>


GLuint loadTexture(const std::string& p_file)
{
	auto format = FreeImage_GetFileType(p_file.c_str());
	auto img_ptr = FreeImage_Load(format, p_file.c_str());

	// Convert to 32bit BGRA:
	auto temp = img_ptr;
	img_ptr = FreeImage_ConvertTo32Bits(img_ptr);
	FreeImage_Unload(temp);
	delete temp;

	std::size_t width = FreeImage_GetWidth(img_ptr),
			    height = FreeImage_GetHeight(img_ptr);

	// Convert to OpenGL texture:
	GLubyte* texture = new GLubyte[4 * width * height];
	char* pixels = (char*)FreeImage_GetBits(img_ptr);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture);

	// Configure OpenGL texture generation:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete texture;
	return texture_id;
}
