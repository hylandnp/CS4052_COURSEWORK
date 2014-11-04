/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Resources/ResourceBase.hpp"


ResourceBase::ResourceBase(ResourceType p_type) :
	m_is_loaded(false),
	m_type(p_type)
{
	// Default initialisation...
}


ResourceBase::~ResourceBase()
{
	// Garbage collection...
	this->unLoad();
}


void ResourceBase::unLoad()
{
	m_type = ResourceType::INVALID;
	m_is_loaded = false;
}


bool ResourceBase::isLoadedAndValid()
{
	return (m_is_loaded && m_type != ResourceType::INVALID);
}


ResourceType ResourceBase::getType()
{
	return m_type;
}
