/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

General OpenGL-related headers and utility functions.

NOTE: The version of GLEW being used is the 'GLEW_MX' compiled version,
	  this allows multiple GLEW contexts for different render windows of
	  different OpenGL configurations.
*/
#pragma once
#include "OpenGL.hpp"

#include <iostream>
#include <string>


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
	std::cerr << "\nERROR: The GLFW library has encountered an error!\n\tCode: '" << p_err_code << "'.\n\tMessage: '" << p_err_msg << "'.\n\n";
}


bool initGLEWContext(GLEWContext* p_glew_context)
{
	// Close previous instance:
	if (p_glew_context) delete p_glew_context;

	// Attempt to create a new GLEW context object for the currently-active window:
	p_glew_context = new GLEWContext();
	GLenum err = 0;

	if ((err = glewContextInit(p_glew_context)) != GLEW_OK)
	{
		std::cerr << "\nERROR: The GLEW library has failed to create/initialise a new context!\n\tMessage: '" << glewGetErrorString(err) << "'.\n\n";

		delete p_glew_context; // dispose of the failed object
		return false;
	}
	else
	{
		std::cout << "Successfully created/initialised a new GLEW window context...";
		return true;
	}
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
			std::cout << "\nERROR: " << err_msg.c_str() << "\n\tIn file: " << p_file_name << "'\n\tAt line: " << p_line_no << ".\n\n";
			current_err = false;
		}
	}

	return has_found_err;
}


// TODO - shader errors
