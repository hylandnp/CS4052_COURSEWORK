/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Resources/ResourceShader.hpp"
#include "Resources/ResourceImage.hpp"
#include "Resources/ResourceTexture2D.hpp"
#include <glm/glm.hpp>


ResourceShader::ResourceShader() :
	m_program_handle(0),
	m_vert_shader(0),
	m_frag_shader(0),
	m_geom_shader(0),
	m_tese_shader(0),
	m_tesc_shader(0)
{
	// Default initialisation...
}


// TODO 
