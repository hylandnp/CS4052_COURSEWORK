/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Window class (wrapper for GLFW and GLEW setup).
*/
#include "Window.hpp"
#include "OpenGL.hpp"
#include "Log.hpp"


Window::Window() :
	m_glfw_handle(nullptr),
	m_was_destroyed(false),
	m_vsync(false),
	m_visible(false),
	m_width(0),
	m_height(0),
	m_title("?")
{
	// Default initialisation...
	//this->create();
}


Window::Window(std::size_t p_width,
			   std::size_t p_height,
			   const std::string& p_title) :
	m_glfw_handle(nullptr),
	m_was_destroyed(false),
	m_vsync(false),
	m_visible(false)
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
					const std::string& p_title)
{
	// Store new window info:
	m_width = p_width;
	m_height = p_height;
	m_title = p_title;

	Log::getInstance().writeMessage("Creating new GLFW-based window...\n");

	// Setup window creation hints:
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	// TODO - other possible hints

	// Create or reset the GLFW window:
	if (m_glfw_handle) glfwDestroyWindow(m_glfw_handle);

	if (!(m_glfw_handle = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL)))
	{
		Log::getInstance().writeError("Window.cpp",
									  __LINE__,
									  "\tFailed to create the GLFW-based OpenGL render window!\n\n");
		return false;
	}

	// Set window position in the centre of the screen:
	auto monitor_video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_glfw_handle,
					 static_cast<int>(ceilf((monitor_video_mode->width - m_width) / 2.f)),
					 static_cast<int>(ceilf((monitor_video_mode->height - m_height) / 2.f)));

	// Setup GLEW:
	glfwMakeContextCurrent(m_glfw_handle);
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		Log::getInstance().writeError("Window.cpp",
									  __LINE__,
									  "\tFailed to initialise the GLEW library for the current window!\n\tMessage: '%s'.\n\n",
									  glewGetErrorString(err));

		glfwDestroyWindow(m_glfw_handle);
		return false;
	}

	// Output window information:
	Log::getInstance().writeMessage("Created %ux%u window: '%s' successfully!\n",
									m_width,
									m_height,
									m_title.c_str());

	Log::getInstance().writeMessage("Using GLFW version: '%s' and GLEW version: '%s'.\n",
									glfwGetVersionString(),
									glewGetString(GLEW_VERSION));
	
	Log::getInstance().writeMessage("With OpenGL version: '%s' and renderer: '%s'.\n",
									glGetString(GL_VERSION),
									glGetString(GL_RENDERER));

	// Setup window state/input callbacks:
	// TODO

	// Configure OpenGL:
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.0);
	glClearStencil(0);
	glClearAccum(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	// Initially-disabled features:
	glDisable(GL_LIGHTING);

	// Setup OpenGL viewport:
	int fb_width = 0,
		fb_height = 0;

	glfwGetFramebufferSize(m_glfw_handle, &fb_width, &fb_height);
	glViewport(0, 0, fb_width, fb_height);

	// If no OpenGL errors reported, return successfully:
	return !hasOpenGLErrors("Window.cpp", __LINE__);
}


void Window::destroy()
{
	if (m_glfw_handle)
	{
		// Any final OpenGL steps:
		// TODO

		glfwDestroyWindow(m_glfw_handle);
	}

	m_was_destroyed = true;
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


bool Window::isVisible()
{
	return ((m_glfw_handle) ? m_visible : false);
}


void Window::setVsyncEnabled(bool p_vsync)
{
	if (m_glfw_handle)
	{
		m_vsync = p_vsync;

		// Apply vertical sync:
		glfwMakeContextCurrent(m_glfw_handle);
		glfwSwapInterval(((m_vsync) ? 1 : 0));
	}
}


bool Window::isVsyncEnabled()
{
	return ((m_glfw_handle) ? m_vsync : false);
}


void Window::asActiveContext()
{
	if (m_glfw_handle) glfwMakeContextCurrent(m_glfw_handle);
}


bool Window::isOpen()
{
	return ((m_glfw_handle) ? (glfwWindowShouldClose(m_glfw_handle) == GL_FALSE) : false);
}


void Window::close()
{
	if (m_glfw_handle) glfwSetWindowShouldClose(m_glfw_handle, GL_TRUE);
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
		Log::getInstance().writeError("Window.cpp",
									  __LINE__,
									  "\tCannot clear OpenGL buffers, no active window/context!\n\n");
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
	return !hasOpenGLErrors("Window.cpp", __LINE__);
}


void Window::display()
{
	if (m_glfw_handle)
	{
		glfwMakeContextCurrent(m_glfw_handle);
		glfwSwapBuffers(m_glfw_handle);
	}
}


void Window::dispatchEvents()
{
	if (m_glfw_handle) glfwPollEvents();
}
