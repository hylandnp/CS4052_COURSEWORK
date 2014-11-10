/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Scene/SceneNodeBase.hpp"

#include <string>
#include <vector>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

// Forward declaration(s):
class SceneNodeCamera;


/*
// TODO
*/
class SceneNodeTransform : public SceneNodeBase
{
public:
	// Constructor/destructor:
	SceneNodeTransform(const std::string& p_name = "?",
					   SceneNodeBase* p_parent = nullptr);
	~SceneNodeTransform();

	// Inheritable update function:
	virtual void updateAndRender(SceneNodeCamera* p_active_camera);

	// Translation setters(s):
	void setPositionX(float p_new_x);
	void setPositionY(float p_new_y);
	void setPositionZ(float p_new_z);
	void setPosition(float p_new_x, float p_new_y, float p_new_z);
	void setPosition(const glm::vec3& p_new_pos);

	void moveByX(float p_offset_x);
	void moveByY(float p_offset_y);
	void moveByZ(float p_offset_z);
	void moveBy(float p_offset_x, float p_offset_y, float p_offset_z);
	void moveBy(const glm::vec3& p_offset);

	// Rotation setter(s):
	void setRotationX(float p_new_x);
	void setRotationY(float p_new_y);
	void setRotationZ(float p_new_z);
	void setRotation(float p_new_x, float p_new_y, float p_new_z);
	void setRotation(const glm::vec3& p_rotation);

	void rotateByX(float p_offset_x);
	void rotateByY(float p_offset_y);
	void rotateByZ(float p_offset_z);
	void rotateBy(float p_offset_x, float p_offset_y, float p_offset_z);
	void rotateBy(const glm::vec3& p_rotation);

	// Scale setter(s):
	void setScaleX(float p_new_x);
	void setScaleY(float p_new_y);
	void setScaleZ(float p_new_z);
	void setScale(float p_new_x, float p_new_y, float p_new_z);
	void setScale(float p_uniform_scale);
	void setScale(const glm::vec3& p_new_scale);

	void scaleByX(float p_offset_x);
	void scaleByY(float p_offset_y);
	void scaleByZ(float p_offset_z);
	void scaleBy(float p_offset_x, float p_offset_y, float p_offset_z);
	void scaleBy(float p_uniform_offset);
	void scaleBy(const glm::vec3& p_offset);

	// Orientation setters(s):
	//void setOrientation(float p_new_w, float p_new_x, float p_new_y, float p_new_z);
	//void setOrientation(const glm::quat& p_orientation);
	//void setOrientation(const glm::vec4& p_orientation);

	// Translation getter(s):
	float getPositionX();
	float getPositionY();
	float getPositionZ();
	const glm::vec3& getPosition();

	// Rotation getter(s):
	float getRotationX();
	float getRotationY();
	float getRotationZ();
	const glm::vec3& getRotation();

	// Scale getter(s):
	float getScaleX();
	float getScaleY();
	float getScaleZ();
	const glm::vec3& getScale();

	// Orientation getter(s):
	//const glm::quat& getOrientation();

	// Utility function(s):
	void forceRebuildOfMatrices();

	// Other setter(s):
	void setRebuildMatrices(bool p_rebuild = true);
	void setTransformAffectsChildren(bool p_affects_children = true);

	// Other getter(s):
	bool getRebuildMatrices();
	bool getTransformAffectsChildren();
	const glm::mat4& getCachedGlobalMatrix();
	const glm::mat4& getCachedLocalMatrix();
protected:
	bool m_rebuild_matrices,
		 m_transform_affects_children;

	glm::vec3 m_location,
			  m_rotation,
			  m_scale;
	glm::quat m_orientation; // TODO
	glm::mat4 m_cached_global_matrix,
			  m_cached_local_matrix;

	// Internal function(s):
	void rebuildMatrix();
};