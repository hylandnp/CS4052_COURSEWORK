/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

General OpenGL-related headers and utility functions.

NOTE: The version of GLEW being used is the 'GLEW_MX' compiled version,
	  this allows multiple GLEW contexts for different render windows of
	  different OpenGL configurations.
*/
#pragma once
#include <cstddef>

// General OpenGL headers:
#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Handling GLFW setup/shutdown/errors:
bool initGLFW();
void deInitGLFW();
void handleGLFWError(int p_err_code, const char* p_err_msg);

// Setup a window's GLEW context:
bool initGLEWContext(GLEWContext* p_glew_context);

// Check for various OpenGL functionality and extensions:
// TODO

// OpenGL error checking (returns true if no errors occurred):
bool checkForOpenGLErrors(const char* p_file_name = __FILE__,
						  std::size_t p_line_no = __LINE__);

// GLSL shader error checking (returns true if no errors occurred):
bool checkShaderComponent(GLenum p_shader_id);
bool checkShaderProgram(GLenum p_shader_id);
