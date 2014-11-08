/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Scene/SceneNodeBase.hpp"
#include <algorithm>


SceneNodeBase::SceneNodeBase(const std::string& p_name,
							 SceneNodeType p_type,
							 SceneNodeBase* p_parent) :
	m_name(p_name),
	m_type(p_type),
	m_parent(p_parent)
{
	// Default initialisation...
	m_children.clear(); // force emptiness
}


SceneNodeBase::~SceneNodeBase()
{
	// Garbage collection...
	m_parent = nullptr;
	this->removeAllChildren(true);
}


bool SceneNodeBase::updateAndRender(const SceneNodeCamera* p_active_camera)
{
	bool no_error = true;

	// Just pass the update/render along to all child nodes:
	if (!m_children.empty())
	{
		for (std::size_t i = 0; i < m_children.size(); i++)
		{
			if (m_children[i])
			{
				no_error = m_children[i]->updateAndRender(p_active_camera);
			}
		}
	}

	return no_error;
}


SceneNodeBase* SceneNodeBase::getParent()
{
	return m_parent;
}


void SceneNodeBase::setParent(SceneNodeBase* p_new_parent)
{
	if (p_new_parent)
	{
		if (m_parent) m_parent->removeChild(this, false);
		m_parent = p_new_parent;
	}
}


void SceneNodeBase::addChild(SceneNodeBase* p_new_child)
{
	if (p_new_child)
	{
		if (m_parent) p_new_child->setParent(this);
		m_children.emplace_back(p_new_child);
	}
}


void SceneNodeBase::removeChild(const std::string& p_to_remove, bool p_delete)
{
	auto child_ptr = this->getChild(p_to_remove);
	if (child_ptr) this->removeChild(child_ptr, true);
}


void SceneNodeBase::removeChild(SceneNodeBase* p_to_remove, bool p_delete)
{
	if (p_to_remove && !m_children.empty())
	{
		auto iter = std::find(m_children.begin(), m_children.end(), p_to_remove);

		if (iter != m_children.end())
		{
			// Remove from list (and delete if required):
			if (p_delete) delete *iter;
			m_children.erase(iter);
		}
	}
}


void SceneNodeBase::removeAllChildren(bool p_delete)
{
	if (!m_children.empty())
	{
		auto iter = m_children.begin();

		while (iter != m_children.end())
		{
			if (p_delete && *iter) delete *iter;
			iter = m_children.erase(iter);
		}
	}

	m_children.clear();
}


SceneNodeBase* SceneNodeBase::getChild(const std::string& p_name)
{
	if (!m_children.empty())
	{
		auto iter = std::find_if(m_children.begin(),
								 m_children.end(),
								 [p_name](SceneNodeBase* ptr) -> bool
								 {
									 return (ptr->getName().compare(p_name) == 0);
								 });

		return ((*iter) ? *iter : nullptr);
	}
	else
	{
		return nullptr; // no valid child found
	}
}


const std::string& SceneNodeBase::getName()
{
	return m_name;
}


bool SceneNodeBase::isRootNode()
{
	return (m_parent == nullptr);
}


std::size_t SceneNodeBase::countChildren(bool p_recursive)
{
	auto result = m_children.size();

	// Count children's children, etc...:
	if (p_recursive)
	{
		for (auto ptr : m_children) if (ptr) result += ptr->countChildren(true);
	}

	return result;
}


std::string SceneNodeBase::getTreeInfo(unsigned int p_depth)
{
	std::string info_str = "";
	for (unsigned int i = 0; i < p_depth; i++) info_str += "\t";

	info_str += "[NODE: '";
	info_str += m_name;
	info_str += "' TYPE: '";
	
	// Conver type to string:
	switch (m_type)
	{
		case SceneNodeType::TRANSFORMATION:
		{
			info_str += "Transformation";
			break;
		}
		case SceneNodeType::CAMERA:
		{
			info_str += "Camera";
			break;
		}
		case SceneNodeType::MESH_STATIC:
		{
			info_str += "Mesh (Static)";
			break;
		}
		default:
		{
			info_str += "Invalid/Unspecified";
			break;
		}
	}
	
	info_str += "'";
	info_str += moreNodeInfo();
	info_str += "]\n";

	// Get child node info:
	if (!m_children.empty())
	{
		for (auto ptr : m_children) if (ptr) info_str += ptr->getTreeInfo(p_depth + 1);
	}

	return info_str;
}


std::string SceneNodeBase::moreNodeInfo()
{
	return std::string();
}
