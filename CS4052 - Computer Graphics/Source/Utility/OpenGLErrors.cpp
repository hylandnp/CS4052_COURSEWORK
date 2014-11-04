/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Utility/OpenGLErrors.hpp"
#include "Utility/Logger.hpp"
#include <GL/glew.h>


bool checkForOpenGLErrors(const std::string& p_file_name,
						  const std::string& p_func_name,
						  std::size_t p_line_no)
{
	GLenum err = 0;
	bool has_error = false,
		 print_err = false;
	std::string err_msg;

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		print_err = true;

		// Get error message string:
		switch (err)
		{
			case GL_INVALID_ENUM:
			{
				has_error = true;
				err_msg = "Invalid OpenGL enumeration detected!";
				break;
			}
			case GL_INVALID_VALUE:
			{
				has_error = true;
				err_msg = "Invalid OpenGL value detected!";
				break;
			}
			case GL_INVALID_OPERATION:
			{
				has_error = true;
				err_msg = "Invalid OpenGL operation detected!";
				break;
			}
			case GL_STACK_OVERFLOW:
			{
				has_error = true;
				err_msg = "OpenGL stack overflow detected!";
				break;
			}
			case GL_STACK_UNDERFLOW:
			{
				has_error = true;
				err_msg = "OpenGL stack underflow detected!";
				break;
			}
			case GL_OUT_OF_MEMORY:
			{
				has_error = true;
				err_msg = "OpenGL out-of-memory detected!";
				break;
			}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				has_error = true;
				err_msg = "Invalid OpenGL framebuffer operation detected!";
				break;
			}
			default:
			{
				print_err = false;
				break;
			}
		}

		if (print_err)
		{
			// Log error message:
		}
	}

	return has_error; // successful
}
