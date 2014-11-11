/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Window.hpp"
#include "Utility/Logger.hpp"
#include "Utility/OpenGLErrors.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// Internal GLFW error callback:
void handleGLFWError(int p_err_code, const char* p_err_msg)
{
	Logger::getInstance().writeError("Window.cpp",
									 "handleGLFWError()",
									 __LINE__,
									 "Encountered GLFW error (%i), with message: '%s'!",
									 p_err_code,
									 p_err_msg);
}


Window::Window() :
	m_glfw_handle(nullptr),
	m_was_destroyed(false),
	m_width(0),
	m_height(0),
	m_title("?"),
	m_visible(false),
	m_vsync(false)
{
	// Default initialisation...
}


Window::~Window()
{
	// Garbage collection...
	if (!m_was_destroyed) this->destroy();
}


bool Window::initGLFW()
{
	// Setup error callback:
	glfwSetErrorCallback(&handleGLFWError);

	// Initialise the GLFW library:
	if (glfwInit() == GL_TRUE)
	{
		Logger::getInstance().write("Initialised the GLFW library successfully, version: '%s'.",
									glfwGetVersionString());
		return true; // successful
	}
	else
	{
		glfwTerminate();
		Logger::getInstance().writeError("Window.cpp",
										 "initGLFW()",
										 __LINE__,
										 "Failed to initialise the GLFW library!");
		return false;
	}
}


void Window::deInitGLFW()
{
	glfwTerminate();
}


bool Window::create(std::size_t p_width,
					std::size_t p_height,
					const std::string& p_title)
{
	if (!m_was_destroyed || m_glfw_handle) this->destroy(); // dispose of previous instance

	m_width = p_width;
	m_height = p_height;
	m_title = p_title;

	// Setup window creation hints:
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create or reset the GLFW window object:
	m_glfw_handle = glfwCreateWindow(p_width,
									 p_height,
									 p_title.c_str(),
									 NULL,
									 NULL);

	if (!m_glfw_handle)
	{
		glfwTerminate();
		Logger::getInstance().writeError("Window.cpp",
										 "create()",
										 __LINE__,
										 "Failed to create GLFW-based window!");
		return false;
	}

	// Set window position in the centre of the screen:
	auto monitor_video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_glfw_handle,
					 static_cast<int>(ceilf((monitor_video_mode->width - m_width) / 2.f)),
					 static_cast<int>(ceilf((monitor_video_mode->height - m_height) / 2.f)));

	// Setup GLEW:
	glfwMakeContextCurrent(m_glfw_handle);
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		Logger::getInstance().writeError("Window.cpp",
										 "create()",
										 __LINE__,
										 "Failed to initialise the GLEW library for the current window!\nMessage: '%s'.",
										 glewGetErrorString(err));

		glfwDestroyWindow(m_glfw_handle);
		return false;
	}

	// Output window information:
	Logger::getInstance().write("Created %ux%u window: '%s' successfully!",
								m_width,
								m_height,
								m_title.c_str());

	Logger::getInstance().write("Using GLFW version: '%s' and GLEW version: '%s'.",
								glfwGetVersionString(),
								glewGetString(GLEW_VERSION));

	Logger::getInstance().write("With OpenGL version: '%s' and renderer: '%s'.",
								glGetString(GL_VERSION),
								glGetString(GL_RENDERER));

	glfwSwapInterval(1);

	// Configure OpenGL:
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glClearDepth(1.0);
	glClearStencil(0);
	glClearAccum(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	// Initially-disabled features:
	glDisable(GL_LIGHTING);

	// Setup OpenGL viewport:
	int fb_width = 0,
		fb_height = 0;

	glfwGetFramebufferSize(m_glfw_handle, &fb_width, &fb_height);
	glViewport(0, 0, fb_width, fb_height);

	// If no OpenGL errors reported, return successfully:
	return !checkForOpenGLErrors("Window.cpp", "create()", __LINE__);
}


void Window::destroy()
{
	if (m_glfw_handle)
	{
		// Any final OpenGL steps:
		// TODO

		glfwDestroyWindow(m_glfw_handle);
		m_glfw_handle = nullptr;
	}

	m_width = 0;
	m_height = 0;
	m_title = "?";
	m_was_destroyed = true;
}


bool Window::clear(bool p_colour_buffer,
				   bool p_depth_buffer,
				   bool p_stencil_buffer,
				   bool p_accum_buffer)
{
	if (m_glfw_handle)
	{
		glfwMakeContextCurrent(m_glfw_handle);
	}
	else
	{
		Logger::getInstance().writeError("Window.cpp",
										 "clear()",
										 __LINE__,
										 "Cannot clear OpenGL buffers, no active window/context!");
		return false;
	}

	// Setup clear mask:
	GLbitfield clear_mask = 0;

	if (p_colour_buffer) clear_mask |= GL_COLOR_BUFFER_BIT;
	if (p_depth_buffer) clear_mask |= GL_DEPTH_BUFFER_BIT;
	if (p_stencil_buffer) clear_mask |= GL_STENCIL_BUFFER_BIT;
	if (p_accum_buffer) clear_mask |= GL_ACCUM_BUFFER_BIT;

	// Clear the window buffers:
	glClear(clear_mask);

	// If no OpenGL errors reported, return successfully:
	return !checkForOpenGLErrors("Window.cpp", "clear()", __LINE__);
}


void Window::display()
{
	if (m_glfw_handle)
	{
		// Render buffer to screen:
		glfwMakeContextCurrent(m_glfw_handle);
		glfwSwapBuffers(m_glfw_handle);

		// Dispatch any window events:
		glfwPollEvents();
	}
}


bool Window::isOpen()
{
	return ((m_glfw_handle) ? (glfwWindowShouldClose(m_glfw_handle) == GL_FALSE) : false);
}


void Window::close()
{
	if (m_glfw_handle) glfwSetWindowShouldClose(m_glfw_handle, GL_TRUE);
}


void Window::setActive(bool p_active)
{
	if (m_glfw_handle) glfwMakeContextCurrent(((p_active) ? m_glfw_handle : NULL));
}


void Window::setVisible(bool p_visible)
{
	if (m_glfw_handle)
	{
		m_visible = p_visible;

		// Apply window visibility:
		if (m_visible)
		{
			glfwShowWindow(m_glfw_handle);
		}
		else
		{
			glfwHideWindow(m_glfw_handle);
		}
	}
}


void Window::setVsync(bool p_vsync)
{
	if (m_glfw_handle)
	{
		m_vsync = p_vsync;

		// Apply vertical sync:
		glfwMakeContextCurrent(m_glfw_handle);
		glfwSwapInterval(((m_vsync) ? 1 : 0));
	}
}


bool Window::isActive()
{
	return (m_glfw_handle && (m_glfw_handle == glfwGetCurrentContext()));
}


bool Window::isVisible()
{
	return ((m_glfw_handle) ? m_visible : false);
}


bool Window::hasVsync()
{
	return ((m_glfw_handle) ? m_vsync : false);
}


GLFWwindow* Window::getRawWindowHandle()
{
	return m_glfw_handle;
}


std::size_t Window::getWidth()
{
	return m_width;
}


std::size_t Window::getHeight()
{
	return m_height;
}


const std::string& Window::getTitle()
{
	return m_title;
}
