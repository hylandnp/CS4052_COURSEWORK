/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Utility/NonCopyable.hpp"


/*
Enumerated resource types.
Each type listed below is implemented in a subclass of the engine's resource class.
*/
enum class ResourceType : unsigned char
{
	INVALID = 0,
	IMAGE,
	TEXTURE_2D,
	TEXTURE_3D,
	TEXTURE_CUBEMAP,
	SHADER,
	MESH_STATIC,
	MESH_DYNAMIC
};


/*
Base resource class, inherited by all resource types.
As the different subclasses have varied loading requirements, no virtual load functions are defined.
*/
class ResourceBase : public NonCopyable
{
public:
	// Constructor/destructor:
	ResourceBase(ResourceType p_type = ResourceType::INVALID);
	virtual ~ResourceBase();

	// Resource disposal function:
	void unLoad();

	// Getter function(s):
	bool isLoadedAndValid();
	ResourceType getType();
protected:
	ResourceType m_type;
	bool m_is_loaded;
};
