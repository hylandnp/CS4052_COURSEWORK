/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

OpenGL camera transformation class.
*/
#pragma once
#include "OpenGL.hpp"


class Camera
{
	public:
		// Constructor:
		Camera();
		
		// Getter functions:
		const glm::mat4& transform(const glm::mat4& p_model_matrix);
		glm::mat4 getViewMatrix();

		// Camera control functions:
		void setPerspective(float p_fov,
							float p_ratio,
							float p_min_range = 0.1f,
							float p_max_range = 250.f);

		void lookAt(const glm::vec3& p_camera_pos,
					const glm::vec3& p_obj_pos,
					const glm::vec3& p_orientation = glm::vec3(0.f, 1.f, 0.f));
	private:
		glm::mat4 m_view_matrix,
				  m_proj_matrix,
				  m_final_mvp_matrix;
};
