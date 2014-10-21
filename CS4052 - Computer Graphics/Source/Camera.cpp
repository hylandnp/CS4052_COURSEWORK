/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

OpenGL camera transformation class.
*/
#include "Camera.hpp"
#include "Transform.hpp"


Camera::Camera() :
	m_proj_matrix(1.f),
	m_view_matrix(1.f),
	m_cached_mvp_matrix(1.f),
	m_was_modified(true)
{
	// Default initialisation...
}


const glm::mat4& Camera::transform(const glm::mat4& p_model_matrix)
{
	m_cached_mvp_matrix = m_proj_matrix *
						  m_view_matrix *
						  p_model_matrix;
	return m_cached_mvp_matrix;
}


void Camera::setPerspective(float p_fov,
							float p_width,
							float p_height,
							float p_min_range,
							float p_max_range)
{
	m_proj_matrix = glm::perspective(p_fov, p_width / p_height, p_min_range, p_max_range);
	m_was_modified = true;
}


void Camera::setOrtho(float p_left,
					  float p_right,
				      float p_bottom,
					  float p_top,
					  float p_min_range,
					  float p_max_range)
{
	m_proj_matrix = glm::ortho(p_left,
							   p_right,
							   p_bottom,
							   p_top,
							   p_min_range,
							   p_max_range);
	m_was_modified = true;
}


void Camera::lookAt(const glm::vec3& p_camera_pos,
					const glm::vec3& p_obj_pos,
					const glm::vec3& p_orientation)
{
	m_view_matrix = glm::lookAt(p_camera_pos, p_obj_pos, p_orientation);
	m_was_modified = true;
}


void Camera::setViewport(GLint p_top,
					 	 GLint p_left,
					 	 std::size_t p_width,
						 std::size_t p_height)
{
	glViewport(p_top, p_left, p_width, p_height);
	m_was_modified = true;
}
