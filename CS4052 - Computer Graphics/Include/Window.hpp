/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include <string>
#include <cstddef>

// Forward declaration(s):
struct GLFWwindow;


/*
Window class.
This class encapsulates a GLFW-based window and OpenGL context object.
*/
class Window
{
public:
	// Constructor/destructor:
	Window();
	~Window();

	// GLFW library setup/shutdown:
	static bool initGLFW();
	static void deInitGLFW();

	// Window creation/destructor:
	bool create(std::size_t p_width = 800,
				std::size_t p_height = 600,
				const std::string& p_title = "CS4052 Computer Graphics - NEIL HYLAND (11511677)");
	void destroy();

	// Render function(s):
	bool clear(bool p_color_buffer = true,
			   bool p_depth_buffer = true,
			   bool p_stencil_buffer = true,
			   bool p_accum_buffer = true);
	void display();

	// Control function(s):
	bool isOpen();
	void close();

	// Setter(s):
	void setActive(bool p_active = true);
	void setVisible(bool p_visible = true);
	void setVsync(bool p_vsync = true);

	// Getter(s):
	bool isActive();
	bool isVisible();
	bool hasVsync();
	GLFWwindow* getRawWindowHandle();
	std::size_t getWidth();
	std::size_t getHeight();
	const std::string& getTitle();
private:
	GLFWwindow* m_glfw_handle;
	bool m_was_destroyed;

	std::size_t m_width,
				m_height;
	std::string m_title;
	bool m_visible,
		 m_vsync;
};
