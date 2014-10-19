/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Vertex transformation object.
*/
#pragma once
#include "OpenGL.hpp"


struct Transform
{
	// Default constructor:
	Transform();

	glm::vec3 Location,
			  RotationAxes,
			  Scale;
	float RotationValue;
	glm::mat4 TranslationMatrix;

	void rebuildMatrix();
};
