/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Resources/ResourceTexture2D.hpp"
#include "Resources/ResourceImage.hpp"
#include "Utility/OpenGLErrors.hpp"
#include "Utility/Logger.hpp"

#include <GL/glew.h>


ResourceTexture2D::ResourceTexture2D() :
	m_gl_handle(0),
	m_width(0),
	m_height(0),
	m_has_mipmaps(false)
{
	// Initialisation...
	ResourceBase::ResourceBase(ResourceType::TEXTURE_2D);
}


bool ResourceTexture2D::load(ResourceImage& p_img_src, bool p_generate_mipmaps)
{
	this->unLoad(); // dispose of previous texture

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_gl_handle);
	glBindTexture(GL_TEXTURE_2D, m_gl_handle);

	m_has_mipmaps = p_generate_mipmaps;
	m_width = p_img_src.getWidthInPixels();
	m_height = p_img_src.getHeightInPixels();

	// Generate texture from software image data:
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 m_width,
				 m_height,
				 0,
				 GL_BGRA,
				 GL_UNSIGNED_BYTE,
				 p_img_src.getRawImagePtr());

	// Setup mipmaps & texture filtering:
	if (p_generate_mipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	if (checkForOpenGLErrors("ResourceTexture2D.cpp", "load()", __LINE__)) return false;

	Logger::getInstance().write("Converted software image to OpenGL texture successfully.");
	return (m_is_loaded = true);
}


bool ResourceTexture2D::loadFromFile(const std::string& p_file_src, bool p_generate_mipmaps)
{
	ResourceImage img_src;

	if(!img_src.loadFromFile(p_file_src))
	{
		Logger::getInstance().writeError("ResourceTexture2D.cpp",
										 "loadFromFile()",
										 __LINE__,
										 "Failed to create OpenGL texture from file: '%s'!",
										 p_file_src.c_str());
		return false;
	}

	return this->load(img_src, p_generate_mipmaps);
}


bool ResourceTexture2D::loadFromString(const std::string& p_str_src, bool p_generate_mipmaps)
{
	ResourceImage img_src;
	
	if(!img_src.loadFromString(p_str_src))
	{
		Logger::getInstance().writeError("ResourceTexture2D.cpp",
										 "loadFromMemory()",
										 __LINE__,
										 "Failed to create OpenGL texture from memory!");
		return false;
	}

	return this->load(img_src, p_generate_mipmaps);
}


void ResourceTexture2D::unLoad()
{
	if (glIsTexture(m_gl_handle) == GL_TRUE) glDeleteTextures(1, &m_gl_handle);

	m_gl_handle = 0;
	m_width = 0;
	m_height = 0;
	m_has_mipmaps = false;

	ResourceBase::unLoad();
}


bool ResourceTexture2D::setActive(bool p_active)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ((p_active) ? m_gl_handle : 0));
	return !checkForOpenGLErrors("ResourceTexture2D.cpp", "setActive()", __LINE__);
}


unsigned int ResourceTexture2D::getRawOpenGLHandle()
{
	return m_gl_handle;
}


bool ResourceTexture2D::hasMipMaps()
{
	return m_has_mipmaps;
}
