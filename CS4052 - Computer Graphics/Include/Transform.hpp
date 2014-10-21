/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Vertex transformation object.
*/
#pragma once
#include "OpenGL.hpp"


class Transform
{
	public:
		// Default constructor:
		Transform();

		// Compute and return the transformation matrix:
		const glm::mat4& getMatrix();

		// Modifiy the transformation:
		void setLocation(const glm::vec3& p_location);
		void setRotationAxes(const glm::vec3& p_rotation_axes);
		void setRotationValue(float p_rotation_value, bool p_as_degrees = true);
		void setScale(const glm::vec3& p_scale);

		// Getter functions:
		const glm::vec3& getLocation();
		const glm::vec3& getRotationAxes();
		float getRotationValue(bool p_as_degrees = true);
		const glm::vec3& getScale();
	private:
		glm::vec3 m_location,
				  m_rotation_axes,
				  m_scale;

		float m_rotation_value;
		glm::mat4 m_cached_transform_matrix;
		bool m_was_modified;
};
