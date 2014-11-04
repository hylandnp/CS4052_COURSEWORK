/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Resources/ResourceBase.hpp"
#include <string>
#include <cstddef>

// Forward declaration(s):
class ResourceImage;


/*
OpenGL texture class.
This class represents simple 2D textures stored in video memory.
Converts from 'BGRA' software images to 'RGBA' hardware textures.
*/
class ResourceTexture2D : public ResourceBase
{
public:
	// Constructor:
	ResourceTexture2D();

	// Creation parameters:
	void setCreationParams(/*TODO*/);

	// Load function(s):
	bool load(const ResourceImage& p_img_src);
	bool loadFromFile(const std::string& p_file_src);
	bool loadFromMemory(const std::string& p_str_src);

	// Inherited disposal function:
	void unLoad();

	// Bind function(s):
	bool setActive(bool p_active = true);

	// Getter function(s):
	unsigned int getRawOpenGLHandle();
private:
	unsigned int m_gl_handle;
};
