/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Vertex transformation object.
*/
#include "Transform.hpp"


Transform::Transform() :
	Location(0.f),
	RotationAxes(0.f),
	Scale(1.f),
	RotationValue(0.f),
	TranslationMatrix(1.f)
{
	// Default initialisation...
}


void Transform::rebuildMatrix()
{
	this->TranslationMatrix = glm::translate(this->Location) *
							  glm::rotate(glm::radians(this->RotationValue), this->RotationAxes) *
							  glm::scale(this->Scale);
}
