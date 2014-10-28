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
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* img_raw(0);

	fif = FreeImage_GetFileType(p_file.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(p_file.c_str());
	if (fif == FIF_UNKNOWN) return 0;
	if (FreeImage_FIFSupportsReading(fif)) img_raw = FreeImage_Load(fif, p_file.c_str());
	if (!img_raw) return 0;

	auto temp = img_raw;
	img_raw = FreeImage_ConvertTo32Bits(img_raw);
	FreeImage_Unload(temp);

	auto img_ptr = FreeImage_GetBits(img_raw);

	auto width = FreeImage_GetWidth(img_raw);
	auto height = FreeImage_GetHeight(img_raw);
	if (img_ptr == NULL || width == 0 || height == 0) return 0;

	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, img_ptr);
	//glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(img_raw);
	return texture_id;
}
