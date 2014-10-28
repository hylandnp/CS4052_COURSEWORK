/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Window class (wrapper for GLFW and GLEW setup).
*/
#pragma once
#include <string>

// Forward declaration(s):
struct GLFWwindow;


class Window
{
	public:
		// Default constructor:
		Window();

		// Constructor:
		Window(std::size_t p_width,
			   std::size_t p_height,
			   const std::string& p_title);

		// Destructor:
		~Window();

		// Creation function:
		bool create(std::size_t p_width = 800,
					std::size_t p_height = 600,
					const std::string& p_title = "CS4052 - Computer Graphics (NEIL HYLAND - 11511677)");

		// Manual destruction function:
		void destroy();

		// Visibility functions:
		void setVisible(bool p_visible = true);
		bool isVisible();

		// Vertical synchronisation functions:
		void setVsyncEnabled(bool p_vsync = true);
		bool isVsyncEnabled();

		// Window set active context function:
		void asActiveContext();

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
		void display();

		// Window event polling function:
		void dispatchEvents();

		// Window handle access:
		GLFWwindow* getRawWindowHandle();

		// Info getter functions:
		std::size_t getWidth();
		std::size_t getHeight();
	private:
		GLFWwindow* m_glfw_handle;

		// Window flags:
		bool m_was_destroyed,
			 m_vsync,
			 m_visible;

		// Window info:
		std::size_t m_width,
					m_height;
		std::string m_title;
};
