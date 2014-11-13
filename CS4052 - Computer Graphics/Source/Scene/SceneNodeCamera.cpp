/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Scene/SceneNodeCamera.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>


SceneNodeCamera::SceneNodeCamera(const std::string& p_name,
								 SceneNodeBase* p_parent) :
	m_is_ortho(false),
	m_proj_perspective(glm::perspective(45.f, 4.f / 3.f, 0.1f, 300.f))
{
	// Initialisation...
	SceneNodeTransform(p_name, p_parent);
	setType(SceneNodeType::CAMERA);
}


SceneNodeCamera::~SceneNodeCamera()
{
	// Garbage collection...
	SceneNodeTransform::~SceneNodeTransform();
}


void SceneNodeCamera::updateAndRender(SceneNodeCamera* p_active_camera)
{
	// TODO

	SceneNodeTransform::updateAndRender(p_active_camera);
}


void SceneNodeCamera::setPerspective(float p_fov,
									 std::size_t p_width,
									 std::size_t p_height,
									 float p_near,
									 float p_far)
{
	m_is_ortho = false;

	float ratio = static_cast<float>(p_width) / static_cast<float>(p_height);
	m_proj_perspective = glm::perspective(glm::radians(p_fov),
										  ratio,
										  p_near,
										  p_far);
}


void SceneNodeCamera::setOrtho(std::size_t p_left,
							   std::size_t p_right,
							   std::size_t p_top,
							   std::size_t p_bottom,
							   float p_near,
							   float p_far)
{
	// TODO
}


const glm::mat4& SceneNodeCamera::getCurrentProjMatrix()
{
	return ((m_is_ortho) ? m_proj_ortho : m_proj_perspective);
}


const glm::mat4& SceneNodeCamera::getPerspectiveProjMatrix()
{
	return m_proj_perspective;
}


const glm::mat4& SceneNodeCamera::getOrthoProjMatrix()
{
	return m_proj_ortho;
}
