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

	// Load function(s):
	bool load(ResourceImage& p_img_src, bool p_generate_mipmaps);
	bool loadFromFile(const std::string& p_file_src, bool p_generate_mipmaps);
	bool loadFromString(const std::string& p_str_src, bool p_generate_mipmaps);

	// Inherited disposal function:
	void unLoad();

	// Bind function(s):
	bool setActive(bool p_active = true);

	// Getter function(s):
	unsigned int getRawOpenGLHandle();
	bool hasMipMaps();
private:
	unsigned int m_gl_handle;
	std::size_t m_width,
				m_height;
	bool m_has_mipmaps;
};
