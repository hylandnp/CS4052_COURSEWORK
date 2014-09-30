/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

General OpenGL-related headers and utility functions.
*/
#pragma once
#include "OpenGL.hpp"

#include <iostream>
#include <string>
#include <vector>


bool initGLFW()
{
	// Setup error callback:
	glfwSetErrorCallback(&handleGLFWError);

	if (glfwInit() == GL_TRUE)
	{
		std::cout << "GLFW library initialised successfully...\n";
		return true;
	}
	else
	{
		std::cerr << "\nERROR: Failed to initialise the GLFW library!\n\n";
		return false;
	}
}


void deInitGLFW()
{
	std::cout << "De-initialising the GLFW library...\n";
	glfwTerminate();
}


void handleGLFWError(int p_err_code, const char* p_err_msg)
{
	std::cerr << "\nERROR: The GLFW library has encountered an error!\n\tCode: " << p_err_code << ".\n\tMessage: '" << p_err_msg << "'.\n\n";
}


// TODO - extension checking


bool checkForOpenGLErrors(const char* p_file_name,
						  std::size_t p_line_no)
{
	GLenum opengl_err;
	std::string err_msg;

	bool has_found_err = false,
		 current_err = false;

	// Check each OpenGL error flag in loop:
	while ((opengl_err = glGetError()) != GL_NO_ERROR)
	{
		current_err = false;

		switch (opengl_err)
		{
		case GL_INVALID_ENUM:
		{
			err_msg = "Invalid OpenGL enum detected!";

			has_found_err = true;
			current_err = true;
			break;
		}
		case GL_INVALID_VALUE:
		{
			err_msg = "Invalid OpenGL value detected!";

			has_found_err = true;
			current_err = true;
			break;
		}
		case GL_INVALID_OPERATION:
		{
			err_msg = "Invalid OpenGL operation detected!";

			has_found_err = true;
			current_err = true;
			break;
		}
		case GL_INVALID_FRAMEBUFFER_OPERATION:
		{
			err_msg = "Invalid OpenGL frame buffer operation detected!";

			has_found_err = true;
			current_err = true;
			break;
		}
		case GL_OUT_OF_MEMORY:
		{
			err_msg = "OpenGL out of memory detected!";

			has_found_err = true;
			current_err = true;
			break;
		}
		case GL_STACK_UNDERFLOW:
		{
			err_msg = "OpenGL stack underflow detected!";

			has_found_err = true;
			current_err = true;
			break;
		}
		case GL_STACK_OVERFLOW:
		{
			err_msg = "OpenGL stack overflow detected!";

			has_found_err = true;
			current_err = true;
			break;
		}
		}

		// Print current error message to console:
		if (current_err)
		{
			std::cerr << "\nERROR: " << err_msg.c_str() << "\n\tIn file: '" << p_file_name << "'\n\tAt line: " << p_line_no << ".\n\n";
		}
	}

	return has_found_err;
}


bool checkShaderComponent(GLenum p_shader_handle,
						  const char* p_file_name,
						  std::size_t p_line_no)
{
	if (glIsShader(p_shader_handle) == GL_FALSE)
	{
		std::cerr << "\nERROR: Invalid shader component detected, not bound to integer parameter!\n\tIn file: '" << p_file_name << "'\n\tAt line: " << p_line_no << ".\n\n";
		return false;
	}

	GLint is_ok = 0;
	glGetShaderiv(p_shader_handle, GL_COMPILE_STATUS, &is_ok);

	if (is_ok == GL_FALSE)
	{
		GLint max_buffer = 0;
		glGetShaderiv(p_shader_handle, GL_INFO_LOG_LENGTH, &max_buffer);

		std::vector<char> msg_buffer(max_buffer);
		glGetShaderInfoLog(p_shader_handle, max_buffer, &max_buffer, &msg_buffer[0]);

		std::cerr << "\nERROR: Invalid shader component detected!\n\tMessage:\n" << &msg_buffer[0] << "\tIn file: '" << p_file_name << "'\n\tAt line: " << p_line_no << ".\n\n";
		msg_buffer.clear();

		// Flag the shader for deletion and return after checking general OpenGL error flags:
		glDeleteShader(p_shader_handle);
		return !glCheck(p_file_name, p_line_no);
	}

	return true; // successful
}


bool checkShaderProgram(GLenum p_program_handle,
						const char* p_file_name,
						std::size_t p_line_no)
{
	if (glIsProgram(p_program_handle) == GL_FALSE)
	{
		std::cerr << "\nERROR: Invalid shader program detected, not bound to integer parameter!\n\tIn file: '" << p_file_name << "'\n\tAt line: " << p_line_no << ".\n\n";
		return false;
	}

	GLint is_ok = 0;
	glGetProgramiv(p_program_handle, GL_LINK_STATUS, &is_ok);

	if (is_ok == GL_FALSE)
	{
		GLint max_buffer = 0;
		glGetProgramiv(p_program_handle, GL_INFO_LOG_LENGTH, &max_buffer);

		std::vector<char> msg_buffer(max_buffer);
		glGetProgramInfoLog(p_program_handle, max_buffer, &max_buffer, &msg_buffer[0]);

		std::cerr << "\nERROR: Invalid shader program detected!\n\tMessage:\n" << &msg_buffer[0] << "\n\tIn file: '" << p_file_name << "'\n\tAt line: " << p_line_no << ".\n\n";
		msg_buffer.clear();

		// Flag the program for deletion and return after checking general OpenGL error flags:
		glDeleteProgram(p_program_handle);
		return !glCheck(p_file_name, p_line_no);
	}

	return true; // successful
}
