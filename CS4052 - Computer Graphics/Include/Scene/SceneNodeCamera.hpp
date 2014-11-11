/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Scene/SceneNodeTransform.hpp"
#include <cstddef>
#include <glm/mat4x4.hpp>


class SceneNodeCamera : public SceneNodeTransform
{
public:
	// Constructor/destructor:
	SceneNodeCamera(const std::string& p_name = "?",
					SceneNodeBase* p_parent = nullptr);
	~SceneNodeCamera();

	// Main inheritable function(s):
	void updateAndRender(SceneNodeCamera* p_active_camera);

	// Projection function(s):
	void setPerspective(float p_fov,
						std::size_t p_width,
						std::size_t p_height,
						float p_near = 0.1f,
						float p_far = 300.f);
	void setOrtho(std::size_t p_left,
				  std::size_t p_right,
				  std::size_t p_top,
				  std::size_t p_bottom,
				  float p_near,
				  float p_far);

	// Matrix getter(s):
	const glm::mat4& getCurrentProjMatrix();
	const glm::mat4& getPerspectiveProjMatrix();
	const glm::mat4& getOrthoProjMatrix();
protected:
	bool m_is_ortho;

	glm::mat4 m_proj_perspective,
			  m_proj_ortho,
			  m_cached_vp_matrix;
};
