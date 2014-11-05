/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Utility/OpenGLErrors.hpp"
#include "Utility/Logger.hpp"
#include <GL/glew.h>
#include <vector>


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


bool checkShaderProgram(unsigned int p_program,
						const std::string& p_file_name,
						const std::string& p_func_name,
						std::size_t p_line_no)
{
	if (glIsProgram(p_program) == GL_FALSE)
	{
		Logger::getInstance().writeError(p_file_name,
										 p_func_name,
										 p_line_no,
										 "Invalid shader program detected (%u), not bound to integer parameter!",
										 p_program);
		return false;
	}

	GLint is_ok = 0;
	glGetProgramiv(p_program, GL_LINK_STATUS, &is_ok);

	if (is_ok == GL_FALSE)
	{
		GLint max_buffer = 0;
		glGetProgramiv(p_program, GL_INFO_LOG_LENGTH, &max_buffer);

		// Format shader log message:
		std::vector<char> msg_buffer(max_buffer);
		glGetProgramInfoLog(p_program, max_buffer, &max_buffer, &msg_buffer[0]);

		Logger::getInstance().writeError(p_file_name,
										 p_func_name,
										 p_line_no,
										 "Invalid shader program detected, message: '%s'!",
										 &msg_buffer[0]);
		msg_buffer.clear();

		// Flag the program for deletion and return after checking general OpenGL error flags:
		glDeleteProgram(p_program);
		return !checkForOpenGLErrors(p_file_name, p_func_name, p_line_no);
	}

	return true; // successful
}


bool checkShaderComponent(unsigned int p_shader,
						  const std::string& p_file_name,
						  const std::string& p_func_name,
						  std::size_t p_line_no)
{
	if (glIsShader(p_shader) == GL_FALSE)
	{
		Logger::getInstance().writeError(p_file_name,
										 p_func_name,
										 p_line_no,
										 "Invalid shader component detected (%u), not bound to integer parameter!",
										 p_shader);
		return false;
	}

	GLint is_ok = 0;
	glGetShaderiv(p_shader, GL_COMPILE_STATUS, &is_ok);

	if (is_ok == GL_FALSE)
	{
		GLint max_buffer = 0;
		glGetShaderiv(p_shader, GL_INFO_LOG_LENGTH, &max_buffer);
		
		// Format shader log message:
		std::vector<char> msg_buffer(max_buffer);
		glGetShaderInfoLog(p_shader, max_buffer, &max_buffer, &msg_buffer[0]);

		Logger::getInstance().writeError(p_file_name,
										 p_func_name,
										 p_line_no,
										 "Invalid shader component detected, message: '%s'!",
										 &msg_buffer[0]);
		msg_buffer.clear();

		// Flag the shader for deletion and return after checking general OpenGL error flags:
		glDeleteShader(p_shader);
		return !checkForOpenGLErrors(p_file_name, p_func_name, p_line_no);
	}

	return true; // successful
}
