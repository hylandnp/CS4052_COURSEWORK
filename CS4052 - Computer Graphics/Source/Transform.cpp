/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Vertex transformation object.
*/
#include "Transform.hpp"


Transform::Transform() :
	m_location(0.f),
	m_rotation_axes(1.f, 0.f, 0.f),
	m_scale(1.f),
	m_rotation_value(0.f),
	m_cached_transform_matrix(1.f),
	m_was_modified(true)
{
	// Default initialisation...
}


const glm::mat4& Transform::getMatrix()
{
	if (m_was_modified)
	{
		m_cached_transform_matrix = glm::translate(m_location) *
									glm::rotate(m_rotation_value, m_rotation_axes) *
									glm::scale(m_scale);
		m_was_modified = false;
	}

	return m_cached_transform_matrix;
}


void Transform::setLocation(const glm::vec3& p_location)
{
	m_location = p_location;
	m_was_modified = true;
}


void Transform::setRotationAxes(const glm::vec3& p_rotation_axes)
{
	m_rotation_axes = p_rotation_axes;
	m_was_modified = true;
}


void Transform::setRotationValue(float p_rotation_value, bool p_as_degrees)
{
	m_rotation_value = ((p_as_degrees) ? glm::radians(p_rotation_value) : p_rotation_value);
	m_was_modified = true;
}


void Transform::setScale(const glm::vec3& p_scale)
{
	m_scale = p_scale;
	m_was_modified = true;
}


const glm::vec3& Transform::getLocation()
{
	return m_location;
}


const glm::vec3& Transform::getRotationAxes()
{
	return m_rotation_axes;
}


float Transform::getRotationValue(bool p_as_degrees)
{
	return ((p_as_degrees) ? glm::degrees(m_rotation_value) : m_rotation_value);
}


const glm::vec3& Transform::getScale()
{
	return m_scale;
}
