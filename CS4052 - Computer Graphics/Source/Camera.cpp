/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

OpenGL camera transformation class.
*/
#include "Camera.hpp"
#include "Transform.hpp"


Camera::Camera() :
	m_proj_matrix(1.f),
	m_view_matrix(),
	m_final_mvp_matrix(1.f)
{
	// Initialisation...
}


const glm::mat4& Camera::getTransform(const glm::mat4& p_model_matrix)
{
	m_final_mvp_matrix = m_proj_matrix *
						 m_view_matrix *
						 p_model_matrix;

	return m_final_mvp_matrix;
}


void Camera::setPerspective(float p_fov,
							float p_ratio,
							float p_min_range,
							float p_max_range)
{
	m_proj_matrix = glm::perspective(p_fov, p_ratio, p_min_range, p_max_range);
}


void Camera::lookAt(const glm::vec3& p_camera_pos,
					const glm::vec3& p_obj_pos,
					const glm::vec3& p_orientation)
{
	m_view_matrix = glm::lookAt(p_camera_pos, p_obj_pos, p_orientation);
}
