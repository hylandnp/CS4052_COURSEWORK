/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

General OpenGL-related headers and utility functions.
*/
#pragma once
#include <cstddef>

// General OpenGL headers:
#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// GLFW startup/shutdown:
bool initGLFW();
void deInitGLFW();
void handleGLFWErrors(int p_err_code, const char* p_err_msg);

// OpenGL extension checking (TODO):
bool hadGeometryShaderSupport();
bool hasTesselationShaderSupport();
bool hasFrameBufferSupport();

// OpenGL error checking (returns true if errors are encountered:
bool hasOpenGLErrors(const char* p_file_name, std::size_t p_line_no);

// Macro for checking a particular function call for OpenGL errors (debug build only):
#if defined(_DEBUG) || !defined(NDEBUG)
	#define glDebugCheck(func, file, line) func; hasOpenGLErrors(file, line);
#else
	#define glDebugCheck(func, file, line) func;
#endif

// GLSL shader error checking:
bool isValidShaderComponent(GLuint p_shader_handle,
							const char* p_file_name,
							std::size_t p_line_no);

bool isValidShaderProgram(GLuint p_program_handle,
						  const char* p_file_name,
						  std::size_t p_line_no);
