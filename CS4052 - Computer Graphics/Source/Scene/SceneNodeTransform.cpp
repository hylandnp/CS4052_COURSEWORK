/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Scene/SceneNodeTransform.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <algorithm>


SceneNodeTransform::SceneNodeTransform(const std::string& p_name,
									   SceneNodeBase* p_parent) :
	m_rebuild_matrices(true),
	m_transform_affects_children(true),
	m_location(0.f),
	m_rotation_euler(0.f),
	m_scale(1.f),
	m_rotation_quat(glm::radians(m_rotation_euler)),
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
	m_rotation_euler.x = fmod(p_new_x, 360.f);
	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotationY(float p_new_y)
{
	m_rotation_euler.y = fmod(p_new_y, 360.f);
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotationZ(float p_new_z)
{
	m_rotation_euler.z = fmod(p_new_z, 360.f);
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotation(float p_new_x, float p_new_y, float p_new_z)
{
	m_rotation_euler.x = fmod(p_new_x, 360.f);
	m_rotation_euler.y = fmod(p_new_y, 360.f);
	m_rotation_euler.z = fmod(p_new_z, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotation(const glm::vec3& p_rotation)
{
	m_rotation_euler.x = fmod(p_rotation.x, 360.f);
	m_rotation_euler.y = fmod(p_rotation.y, 360.f);
	m_rotation_euler.z = fmod(p_rotation.z, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotationAsQuaternion(float p_new_w, float p_new_x, float p_new_y, float p_new_z)
{
	m_rotation_quat = glm::quat(glm::radians(p_new_w),
								glm::radians(p_new_x),
								glm::radians(p_new_y),
								glm::radians(p_new_z));

	m_rotation_euler = glm::degrees(glm::eulerAngles(m_rotation_quat));
	m_rotation_euler.x = fmod(m_rotation_euler.x, 360.f);
	m_rotation_euler.y = fmod(m_rotation_euler.y, 360.f);
	m_rotation_euler.z = fmod(m_rotation_euler.z, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::setRotationAsQuaternion(const glm::quat& p_rotation)
{
	m_rotation_quat = p_rotation;

	m_rotation_euler = glm::degrees(glm::eulerAngles(m_rotation_quat));
	m_rotation_euler.x = fmod(m_rotation_euler.x, 360.f);
	m_rotation_euler.y = fmod(m_rotation_euler.y, 360.f);
	m_rotation_euler.z = fmod(m_rotation_euler.z, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateByX(float p_offset_x)
{
	m_rotation_euler.x = fmod(m_rotation_euler.x + p_offset_x, 360.f);
	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateByY(float p_offset_y)
{
	m_rotation_euler.y = fmod(m_rotation_euler.y + p_offset_y, 360.f);
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateByZ(float p_offset_z)
{
	m_rotation_euler.z = fmod(m_rotation_euler.z + p_offset_z, 360.f);
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateBy(float p_offset_x, float p_offset_y, float p_offset_z)
{
	m_rotation_euler.x = fmod(m_rotation_euler.x + p_offset_y, 360.f);
	m_rotation_euler.y = fmod(m_rotation_euler.y + p_offset_y, 360.f);
	m_rotation_euler.z = fmod(m_rotation_euler.z + p_offset_y, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
	m_rebuild_matrices = true;
}


void SceneNodeTransform::rotateBy(const glm::vec3& p_rotation)
{
	m_rotation_euler.x = fmod(m_rotation_euler.x + p_rotation.x, 360.f);
	m_rotation_euler.y = fmod(m_rotation_euler.y + p_rotation.y, 360.f);
	m_rotation_euler.z = fmod(m_rotation_euler.z + p_rotation.z, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;

	m_rotation_quat = glm::quat(glm::radians(m_rotation_euler));
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
	return m_rotation_euler.x;
}


float SceneNodeTransform::getRotationY()
{
	return m_rotation_euler.y;
}


float SceneNodeTransform::getRotationZ()
{
	return m_rotation_euler.z;
}


const glm::vec3& SceneNodeTransform::getRotation()
{
	return m_rotation_euler;
}


const glm::quat& SceneNodeTransform::getRotationAsQuaternion()
{
	return m_rotation_quat;
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


void SceneNodeTransform::lookAt(float p_target_x,
								float p_target_y,
								float p_target_z,
								const glm::vec3& p_up)
{
	m_rotation_quat = glm::quat_cast(glm::lookAt(m_location,
												 glm::vec3(p_target_x, p_target_y, p_target_z),
												 p_up));

	m_rotation_euler = glm::degrees(glm::eulerAngles(m_rotation_quat));
	m_rotation_euler.x = fmod(m_rotation_euler.x, 360.f);
	m_rotation_euler.y = fmod(m_rotation_euler.y, 360.f);
	m_rotation_euler.z = fmod(m_rotation_euler.z, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;
	m_rebuild_matrices = true;
}


void SceneNodeTransform::lookAt(const glm::vec3& p_target,
								const glm::vec3& p_up)
{
	m_rotation_quat = glm::quat_cast(glm::lookAt(m_location,
												 p_target,
												 p_up));

	m_rotation_euler = glm::degrees(glm::eulerAngles(m_rotation_quat));
	m_rotation_euler.x = fmod(m_rotation_euler.x, 360.f);
	m_rotation_euler.y = fmod(m_rotation_euler.y, 360.f);
	m_rotation_euler.z = fmod(m_rotation_euler.z, 360.f);

	if (m_rotation_euler.x < 0.f) m_rotation_euler.x += 360.f;
	if (m_rotation_euler.y < 0.f) m_rotation_euler.y += 360.f;
	if (m_rotation_euler.z < 0.f) m_rotation_euler.z += 360.f;
	m_rebuild_matrices = true;
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
	m_cached_local_matrix = glm::mat4(1.f);

	// Translation:
	m_cached_local_matrix = glm::translate(m_cached_local_matrix, m_location);

	// Rotation (quaternion):
	m_cached_local_matrix *= glm::toMat4(m_rotation_quat);

	// Scaling:
	m_cached_local_matrix = glm::scale(m_cached_local_matrix, m_scale);

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
