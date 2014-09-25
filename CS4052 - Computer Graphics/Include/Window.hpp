/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Window class (wrapper for GLFW and GLEW setup).
*/
#pragma once
#include <string>

// Forward declaration(s):
struct GLFWwindow;
struct GLEWContextStruct;


class Window
{
public:
	// Default constructor:
	Window();

	// Constructor:
	Window(std::size_t p_width,
		   std::size_t p_height,
		   const std::string& p_title,
		   Window* p_shared);

	// Destructor:
	~Window();

	// (Re)creation function:
	bool create(std::size_t p_width = 1024,
				std::size_t p_height = 768,
				const std::string& p_title = "CS4051 - Computer Graphics (NEIL HYLAND - 11511677)",
				Window* p_shared = nullptr);

	// Manual destruction function:
	bool destroy();

	// Visibility functions:
	void setVisible(bool p_visible = true);
	bool isVisible();

	// Vertical synchronisation functions:
	void setVsyncEnabled(bool p_vsync = true);
	bool isVsyncEnabled();

	// Window set active context function:
	bool asActiveContext();

	// Window is running check function:
	bool isOpen();

	// Window close flag set function:
	void close();

	// Window clear buffer function:
	bool clear(bool p_colour_buffer = true,
			   bool p_depth_buffer = true,
			   bool p_stencil_buffer = true,
			   bool p_accum_buffer = true);

	// Window swap buffers function:
	bool display();

	// Window event polling function:
	void dispatchEvents();

	// GLFW global initialisation function:
	static bool initGLFW();

	// GLFW global de-initialisation function:
	static void deInitGLFW();
private:
	GLFWwindow* m_glfw_handle;
	GLEWContextStruct* m_glew_handle;

	// Window flags:
	bool m_was_destroyed,
		 m_vsync,
		 m_visible;

	// Window info:
	std::size_t m_width,
		m_height;
	std::string m_title;
};
