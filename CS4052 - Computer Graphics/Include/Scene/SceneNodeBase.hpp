/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include <string>
#include <vector>

// Forward declaration(s):
class SceneNodeCamera;


/*
Enumerated node types:
*/
enum class SceneNodeType : unsigned char
{
	INVALID = 0,
	TRANSFORM,
	CAMERA,
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOTLIGHT,
	MESH_STATIC,
};



/*
Base scene node class.
All scene node objects inherit from this base class.
*/
class SceneNodeBase
{
public:
	// Constructor/destructor:
	SceneNodeBase(const std::string& p_name = "?",
				  SceneNodeType p_type = SceneNodeType::INVALID,
				  SceneNodeBase* p_parent = nullptr);
	virtual ~SceneNodeBase();

	// Main inheritable function(s):
	virtual bool updateAndRender(const SceneNodeCamera* p_active_camera);

	// Parent function(s):
	SceneNodeBase* getParent();
	void setParent(SceneNodeBase* p_new_parent);

	// Child function(s):
	void addChild(SceneNodeBase* p_new_child);
	void removeChild(const std::string& p_to_remove, bool p_delete = true);
	void removeChild(SceneNodeBase* p_to_remove, bool p_delete = true);
	void removeAllChildren(bool p_delete = true);
	SceneNodeBase* getChild(const std::string& p_child_name);

	// Node info getter(s) :
	const std::string& getName();
	bool isRootNode();
	SceneNodeType getType();
	std::size_t countChildren(bool p_recursive = false);

	// Debug info function(s):
	std::string getTreeInfo(unsigned int p_depth = 0);
protected:
	std::string m_name;
	SceneNodeType m_type;

	// Node pointers:
	SceneNodeBase* m_parent;
	std::vector<SceneNodeBase*> m_children;

	// Internal inheritable debug info:
	virtual std::string moreNodeInfo();
	void setType(SceneNodeType p_new_type);
};
