/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

General OpenGL-related headers and utility functions.
*/
#include "OpenGL.hpp"
#include "Log.hpp"

#include <iostream>
#include <vector>


bool initGLFW()
{
	glfwSetErrorCallback(&handleGLFWErrors);

	if(glfwInit() == GL_TRUE)
	{
		Log::getInstance().writeMessage("Successfullt initialised the GLFW library...\n");
		return true;
	}
	else
	{
		Log::getInstance().writeError(__FILE__,
								  	  __LINE__,
								  	  "\tFailed to initialise the GLFW library!\n\n");
		return false;
	}
}


void deInitGLFW()
{
	Log::getInstance().writeMessage("De-initialising the GLFW library...\n");
	glfwTerminate();
}


void handleGLFWErrors(int p_err_code, const char* p_err_msg)
{
	Log::getInstance().writeError(__FILE__,
								  __LINE__,
								  "\tThe GLFW library has encountered an error!\n\tCode: %i.\n\tMessage: '%s'.\n\n",
								  p_err_code,
								  p_err_msg);
}


bool hasGeometryShaderSupport()
{
	return true; // TODO
}


bool hasTesselationShaderSupport()
{
	return true; // TODO
}


bool hasFrameBufferSupport()
{
	return true; // TODO
}


bool hasOpenGLErrors(const char* p_file_name, std::size_t p_line_no)
{
	GLenum opengl_err;
	bool has_found_err = false;

	while((opengl_err = glGetError()) != GL_NO_ERROR)
	{
		switch(opengl_err)
		{
			case GL_INVALID_ENUM:
			{
				Log::getInstance().writeError(p_file_name,
											  p_line_no,
											  "\tInvalid OpenGL enumeration detected!\n\n");

				has_found_err = true;
				break;
			}
			case GL_INVALID_VALUE:
			{
				Log::getInstance().writeError(p_file_name,
											  p_line_no,
											  "\tInvalid OpenGL value detected!\n\n");

				has_found_err = true;
				break;
			}
			case GL_INVALID_OPERATION:
			{
				Log::getInstance().writeError(p_file_name,
											  p_line_no,
											  "\tInvalid OpenGL operation detected!\n\n");

				has_found_err = true;
				break;
			}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				Log::getInstance().writeError(p_file_name,
											  p_line_no,
											  "\tInvalid OpenGL framebuffer operation detected!\n\n");

				has_found_err = true;
				break;
			}
			case GL_OUT_OF_MEMORY:
			{
				Log::getInstance().writeError(p_file_name,
											  p_line_no,
											  "\tInvalid OpenGL out of memory detected!\n\n");

				has_found_err = true;
				break;
			}
			case GL_STACK_UNDERFLOW:
			{
				Log::getInstance().writeError(p_file_name,
											  p_line_no,
											  "\tInvalid OpenGL stack underflow detected!\n\n");

				has_found_err = true;
				break;
			}
			case GL_STACK_OVERFLOW:
			{
				Log::getInstance().writeError(p_file_name,
											  p_line_no,
											  "\tInvalid OpenGL stack overflow detected!\n\n");

				has_found_err = true;
				break;
			}
		}
	}

	return has_found_err;
}


bool isValidShaderComponent(GLuint p_shader_handle,
							const char* p_file_name,
							std::size_t p_line_no)
{
	if (glIsShader(p_shader_handle) == GL_FALSE)
	{
		Log::getInstance().writeError(p_file_name,
									  p_line_no,
									  "\tInvalid shader component detected (%u), not bound to integer parameter!\n\n",
									  p_shader_handle);
		return false;
	}

	GLint is_ok = 0;
	glGetShaderiv(p_shader_handle, GL_COMPILE_STATUS, &is_ok);

	if (is_ok == GL_FALSE)
	{
		GLint max_buffer = 0;
		glGetShaderiv(p_shader_handle, GL_INFO_LOG_LENGTH, &max_buffer);

		// Format shader log message:
		std::vector<char> msg_buffer(max_buffer);
		glGetShaderInfoLog(p_shader_handle, max_buffer, &max_buffer, &msg_buffer[0]);

		Log::getInstance().writeError(p_file_name,
									  p_line_no,
									  "\tInvalid shader component detected (%u)!\n\tMessage: '%s'.\n\n",
									  p_shader_handle,
									  &msg_buffer[0]);
		msg_buffer.clear();

		// Flag the shader for deletion and return after checking general OpenGL error flags:
		glDeleteShader(p_shader_handle);
		return !hasOpenGLErrors(p_file_name, p_line_no);
	}

	return true; // successful
}


bool isValidShaderProgram(GLuint p_program_handle,
						  const char* p_file_name,
						  std::size_t p_line_no)
{
	if (glIsProgram(p_program_handle) == GL_FALSE)
	{
		Log::getInstance().writeError(p_file_name,
									  p_line_no,
									  "\tInvalid shader program detected (%u), not bound to integer parameter!\n\n",
									  p_program_handle);
		return false;
	}

	GLint is_ok = 0;
	glGetProgramiv(p_program_handle, GL_LINK_STATUS, &is_ok);

	if (is_ok == GL_FALSE)
	{
		GLint max_buffer = 0;
		glGetProgramiv(p_program_handle, GL_INFO_LOG_LENGTH, &max_buffer);

		// Format program log message:
		std::vector<char> msg_buffer(max_buffer);
		glGetProgramInfoLog(p_program_handle, max_buffer, &max_buffer, &msg_buffer[0]);

		Log::getInstance().writeError(p_file_name,
									  p_line_no,
									  "\tInvalid shader program detected (%u)!\n\tMessage: '%s'.\n\n",
									  p_program_handle,
									  &msg_buffer[0]);
		msg_buffer.clear();

		// Flag the program for deletion and return after checking general OpenGL error flags:
		glDeleteProgram(p_program_handle);
		return !hasOpenGLErrors(p_file_name, p_line_no);
	}

	return true; // successful
}

