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
	GLenum opengl_err;
	bool has_found_err = false;

	while((opengl_err = glGetError()) != GL_NO_ERROR)
	{
		// Log error messages for detected OpenGL errors.
		switch(opengl_err)
		{
			case GL_INVALID_ENUM:
			{
				Logger::getInstance().writeError(p_file_name,
												 p_func_name,
												 p_line_no,
												 "Invalid OpenGL enumeration detected!");

				has_found_err = true;
				break;
			}
			case GL_INVALID_VALUE:
			{
				Logger::getInstance().writeError(p_file_name,
												 p_func_name,
											 	 p_line_no,
											 	 "Invalid OpenGL value detected!");

				has_found_err = true;
				break;
			}
			case GL_INVALID_OPERATION:
			{
				Logger::getInstance().writeError(p_file_name,
												 p_func_name,
											 	 p_line_no,
											 	 "Invalid OpenGL operation detected!");

				has_found_err = true;
				break;
			}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				Logger::getInstance().writeError(p_file_name,
												 p_func_name,
											  	 p_line_no,
											 	 "Invalid OpenGL framebuffer operation detected!");

				has_found_err = true;
				break;
			}
			case GL_OUT_OF_MEMORY:
			{
				Logger::getInstance().writeError(p_file_name,
												 p_func_name,
											  	 p_line_no,
											  	 "OpenGL out-of-memory detected!");

				has_found_err = true;
				break;
			}
			case GL_STACK_UNDERFLOW:
			{
				Logger::getInstance().writeError(p_file_name,
												 p_func_name,
											 	 p_line_no,
											     "OpenGL stack underflow detected!");

				has_found_err = true;
				break;
			}
			case GL_STACK_OVERFLOW:
			{
				Logger::getInstance().writeError(p_file_name,
												 p_func_name,
											  	 p_line_no,
											     "Invalid OpenGL stack overflow detected!");

				has_found_err = true;
				break;
			}
		}
	}

	return has_found_err;
}
