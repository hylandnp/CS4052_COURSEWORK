/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Resources/ResourceTexture2D.hpp"
#include "Resources/ResourceImage.hpp"
#include "Utility/OpenGLErrors.hpp"
#include <GL/glew.h>


ResourceTexture2D::ResourceTexture2D()
{
	// Initialisation...
	ResourceBase::ResourceBase(ResourceType::TEXTURE_2D);
}


void ResourceTexture2D::setCreationParams()
{
	// TODO
}


bool ResourceTexture2D::load(const ResourceImage& p_img_src)
{
	// TODO

	return (m_is_loaded = true);
}


bool ResourceTexture2D::loadFromFile(const std::string& p_file_src)
{
	ResourceImage img_src;

	if(img_src.loadFromFile(p_file_src))
	{
		// TODO - err message
		return false;
	}

	return this->load(img_src);
}


bool ResourceTexture2D::loadFromMemory(const std::string& p_str_src)
{
	ResourceImage img_src;
	
	if(img_src.loadFromMemory(p_str_src))
	{
		// TODO - err message
		return false;
	}

	return this->load(img_src);
}


void ResourceTexture2D::unLoad()
{
	if (glIsTexture(m_gl_handle) == GL_TRUE) glDeleteTextures(1, &m_gl_handle);
	m_gl_handle = 0;

	ResourceBase::unLoad();
}


bool ResourceTexture2D::setActive(bool p_active)
{
	glBindTexture(((p_active) ? m_gl_handle : 0), GL_TEXTURE_2D);
	return !checkForOpenGLErrors("ResourceTexture2D.cpp", "setActive(bool)", __LINE__);
}


unsigned int ResourceTexture2D::getRawOpenGLHandle()
{
	return m_gl_handle;
}
