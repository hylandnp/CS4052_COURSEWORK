/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

General OpenGL-related headers and utility functions.
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


// Check for various OpenGL functionality and extensions:
bool hasGeometryShaderSupport();
bool hasTesselationShaderSupport();
bool hasFrameBufferSupport();
// TODO


// OpenGL error checking (returns true if errors have occurred):
bool checkForOpenGLErrors(const char* p_file_name,
						  std::size_t p_line_no);


// GLSL shader error checking (returns true if no errors occurred):
bool checkShaderComponent(GLenum p_shader_handle,
						  const char* p_file_name,
						  std::size_t p_line_no);

bool checkShaderProgram(GLenum p_program_handle,
						const char* p_file_name,
						std::size_t p_line_no);

// OpenGL error checking macros:
#define glCheck(file, line) !checkForOpenGLErrors(file, line);
#define glShaderCheck(shader) checkShaderComponent(shader, __FILE__, __LINE__);
#define glProgramCheck(program) checkShaderProgram(program, __FILE__, __LINE__);
