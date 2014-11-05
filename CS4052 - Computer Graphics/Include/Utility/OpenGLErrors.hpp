/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include <string>
#include <cstddef>


// Function to poll OpenGL error states:
bool checkForOpenGLErrors(const std::string& p_file_name,
						  const std::string& p_func_name,
						  std::size_t p_line_no);

// Function(s) to check GLSL shaders (returns true if OK):
bool checkShaderProgram(unsigned int p_program,
						const std::string& p_file_name,
						const std::string& p_func_name,
						std::size_t p_line_no);
bool checkShaderComponent(unsigned int p_shader,
						  const std::string& p_file_name,
						  const std::string& p_func_name,
						  std::size_t p_line_no);
