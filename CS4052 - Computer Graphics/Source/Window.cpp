/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Window class (wrapper for GLFW and GLEW setup).
*/
#include "Window.hpp"
#include "OpenGL.hpp"

#include <iostream>


Window::Window() :
	m_glfw_handle		(nullptr),
	m_was_destroyed		(false),
	m_vsync				(false),
	m_visible			(false),
	m_width				(0),
	m_height			(0),
	m_title				("?")
{
	// Default initialisation...
	this->create();
}


Window::Window(std::size_t p_width,
			   std::size_t p_height,
			   const std::string& p_title,
			   Window* p_shared) :
	m_glfw_handle		(nullptr),
	m_was_destroyed		(false),
	m_vsync				(false),
	m_visible			(false)
{
	// Initialisation...
	this->create(p_width, p_height, p_title);
}


Window::~Window()
{
	// Garbage collection...
	if (!m_was_destroyed) this->destroy();
}


bool Window::create(std::size_t p_width,
					std::size_t p_height,
					const std::string& p_title,
					Window* p_shared)
{
	// TODO
	return true;
}


bool Window::destroy()
{
	// TODO
	return (m_was_destroyed = true);
}