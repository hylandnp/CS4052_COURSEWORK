/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Scene/SceneNodeTransform.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>


SceneNodeTransform::SceneNodeTransform(const std::string& p_name,
									   SceneNodeBase* p_parent) :
	m_rebuild_matrices(true),
	m_transform_affects_children(true),
	m_location(0.f),
	m_rotation(0.f),
	m_scale(1.f),
	m_orientation(0.f, 0.f, 0.f, 0.f),
	m_cached_local_matrix(1.f),
	m_cached_global_matrix(1.f)
{
	// Initialisation...
	SceneNodeBase(p_name, SceneNodeType::TRANSFORM, p_parent);
}


SceneNodeTransform::~SceneNodeTransform()
{
	// Garbage collection...
	SceneNodeBase::~SceneNodeBase();
}


void SceneNodeTransform::updateAndRender(SceneNodeCamera* p_active_camera)
{
	if (m_rebuild_matrices) rebuildMatrix();

	// TODO

	SceneNodeBase::updateAndRender(p_active_camera);
}


void SceneNodeTransform::setPositionX(float p_new_x)
{
	m_location.x = p_new_x;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setPositionY(float p_new_y)
{
	m_location.y = p_new_y;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setPositionZ(float p_new_z)
{
	m_location.z = p_new_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setPosition(float p_new_x, float p_new_y, float p_new_z)
{
	m_location.x = p_new_x;
	m_location.y = p_new_y;
	m_location.z = p_new_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setPosition(const glm::vec3& p_new_pos)
{
	m_location = p_new_pos;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::moveByX(float p_offset_x)
{
	m_location.x += p_offset_x;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::moveByY(float p_offset_y)
{
	m_location.y += p_offset_y;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::moveByZ(float p_offset_z)
{
	m_location.z += p_offset_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::moveBy(float p_offset_x, float p_offset_y, float p_offset_z)
{
	m_location.x += p_offset_x;
	m_location.y += p_offset_y;
	m_location.z += p_offset_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::moveBy(const glm::vec3& p_offset)
{
	m_location += p_offset;
	m_rebuild_matrices;
}


void SceneNodeTransform::setRotationX(float p_new_x)
{
	m_rotation.x = p_new_x;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotationY(float p_new_y)
{
	m_rotation.y = p_new_y;
	m_rebuild_matrices = true;
}

void SceneNodeTransform::setRotationZ(float p_new_z)
{
	m_rotation.z = p_new_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotation(float p_new_x, float p_new_y, float p_new_z)
{
	m_rotation.x = p_new_x;
	m_rotation.y = p_new_y;
	m_rotation.z = p_new_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotation(const glm::vec3& p_rotation)
{
	m_rotation = p_rotation;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateByX(float p_offset_x)
{
	m_rotation.x += p_offset_x;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateByY(float p_offset_y)
{
	m_rotation.y += p_offset_y;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateByZ(float p_offset_z)
{
	m_rotation.z += p_offset_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateBy(float p_offset_x, float p_offset_y, float p_offset_z)
{
	m_rotation.x += p_offset_x;
	m_rotation.y += p_offset_y;
	m_rotation.z += p_offset_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateBy(const glm::vec3& p_rotation)
{
	m_rotation += p_rotation;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setScaleX(float p_new_x)
{
	m_scale.x = p_new_x;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setScaleY(float p_new_y)
{
	m_scale.y = p_new_y;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setScaleZ(float p_new_z)
{
	m_scale.z = p_new_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setScale(float p_new_x, float p_new_y, float p_new_z)
{
	m_scale.x = p_new_x;
	m_scale.y = p_new_y;
	m_scale.z = p_new_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setScale(float p_uniform_scale)
{
	m_scale = glm::vec3(p_uniform_scale);
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setScale(const glm::vec3& p_new_scale)
{
	m_scale = p_new_scale;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::scaleByX(float p_offset_x)
{
	m_scale.x += p_offset_x;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::scaleByY(float p_offset_y)
{
	m_scale.y += p_offset_y;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::scaleByZ(float p_offset_z)
{
	m_scale.z += p_offset_z;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::scaleBy(float p_uniform_offset)
{
	m_scale += p_uniform_offset;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::scaleBy(const glm::vec3& p_offset)
{
	m_scale += p_offset;
	m_rebuild_matrices = true;
}


float SceneNodeTransform::getPositionX()
{
	return m_location.x;
}


float SceneNodeTransform::getPositionY()
{
	return m_location.y;
}


float SceneNodeTransform::getPositionZ()
{
	return m_location.z;
}


const glm::vec3& SceneNodeTransform::getPosition()
{
	return m_location;
}


float SceneNodeTransform::getRotationX()
{
	return m_rotation.x;
}


float SceneNodeTransform::getRotationY()
{
	return m_rotation.y;
}


float SceneNodeTransform::getRotationZ()
{
	return m_rotation.z;
}


const glm::vec3& SceneNodeTransform::getRotation()
{
	return m_rotation;
}


float SceneNodeTransform::getScaleX()
{
	return m_scale.x;
}


float SceneNodeTransform::getScaleY()
{
	return m_scale.y;
}


float SceneNodeTransform::getScaleZ()
{
	return m_scale.z;
}


const glm::vec3& SceneNodeTransform::getScale()
{
	return m_scale;
}


void SceneNodeTransform::forceRebuildOfMatrices()
{
	rebuildMatrix();
}


void SceneNodeTransform::setRebuildMatrices(bool p_rebuild)
{
	m_rebuild_matrices = p_rebuild;
}


void SceneNodeTransform::setTransformAffectsChildren(bool p_affects_children)
{
	m_transform_affects_children = p_affects_children;
}


bool SceneNodeTransform::getRebuildMatrices()
{
	return m_rebuild_matrices;
}


bool SceneNodeTransform::getTransformAffectsChildren()
{
	return m_transform_affects_children;
}


const glm::mat4& SceneNodeTransform::getCachedGlobalMatrix()
{
	if (m_rebuild_matrices) rebuildMatrix();
	return m_cached_global_matrix;
}


const glm::mat4& SceneNodeTransform::getCachedLocalMatrix()
{
	if (m_rebuild_matrices) rebuildMatrix();
	return m_cached_local_matrix;
}


void SceneNodeTransform::rebuildMatrix()
{
	// Translation:
	m_cached_local_matrix = glm::translate(glm::mat4(1.f), m_location);
	//m_cached_local_matrix = glm::inverse(glm::lookAt(m_location, glm::vec3(10.f, 10.f, 0.f), glm::vec3(0.f, 1.f, 0.f)));

	// Rotation:
	m_cached_local_matrix = glm::rotate(m_cached_local_matrix,
										glm::radians(m_rotation.x),
										glm::vec3(1.f, 0.f, 0.f));
	m_cached_local_matrix = glm::rotate(m_cached_local_matrix,
										glm::radians(m_rotation.y),
										glm::vec3(0.f, 1.f, 0.f));
	m_cached_local_matrix = glm::rotate(m_cached_local_matrix,
										glm::radians(m_rotation.z),
										glm::vec3(0.f, 0.f, 1.f));

	//m_cached_local_matrix *= glm::toMat4(glm::quat(glm::radians(m_rotation)));

	// Scaling:
	m_cached_local_matrix = glm::scale(m_cached_local_matrix, m_scale);
	//m_cached_local_matrix = glm::inverse(m_cached_local_matrix);

	// Compute 'global' matrix position relative to game scene:
	//if (m_parent &&
	//	m_parent->getType() != SceneNodeType::INVALID &&
	//	reinterpret_cast<SceneNodeTransform*>(m_parent)->getTransformAffectsChildren())
	//{
	//	m_cached_global_matrix = reinterpret_cast<SceneNodeTransform*>(m_parent)->getCachedGlobalMatrix() *
	//							 m_cached_local_matrix;
	//}
	//else
	{
		// Global matrix is the same as the local matrix:
		m_cached_global_matrix = m_cached_local_matrix;
	}

	m_rebuild_matrices = false;
}
