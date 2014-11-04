/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include <string>
#include <cstddef>


/*
Function to poll OpenGL error states.
*/
bool checkForOpenGLErrors(const std::string& p_file_name,
						  const std::string& p_func_name,
						  std::size_t p_line_no);
