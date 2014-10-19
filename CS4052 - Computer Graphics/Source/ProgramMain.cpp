/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Program entry point ('main' function).
*/
#include "OpenGL.hpp"
#include "Log.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Input.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

#include <iostream>
#include <vector>


#if (defined(_WIN32) || defined(WIN32)) && !defined(_CONSOLE)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	// If application build type is Win32 GUI or any other non-console type, use the Windows entry point function:
	int WINAPI WinMain(HINSTANCE p_curr_inst, HINSTANCE p_prev_inst, LPSTR p_cmd_line, int p_cmd_show)
#else
	// Otherwise use regular entry point function:
	int main(int p_argc, char** p_argv)
#endif
{
	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << "Entering main function...\n";
	#endif // _DEBUG || !NDEBUG

	// Startup:
	Log::getInstance().openFile("cs4051.log");
	Input::getInstance().clearInputs();

	if (!initGLFW()) return EXIT_FAILURE;
	Window* game_window = new Window();

	if (!game_window->create(800, 600))
	{
		delete game_window;
		return EXIT_FAILURE;
	}

	game_window->asActiveContext();

	// Load shaders:
	Shader* simple_shader = new Shader();
	if (!simple_shader->loadFromFile("Assets/SimpleShader.vert.glsl",
									 "Assets/SimpleShader.frag.glsl"))
	{
		return EXIT_FAILURE;
	}

	// Begin runtime:
	game_window->setVisible();
	Log::getInstance().writeMessage("Entering main render loop...\n");

	// Set framerate calculations:
	double last_frame = glfwGetTime(),
		   current_frame = 0.f,
		   delta_time = 0.f;
	
	while (game_window->isOpen())
	{
		// Control delta time:
		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// Clear the window:
		if (!game_window->clear()) return EXIT_FAILURE;

		// Render the game scene:
		// TODO

		// Swap the window buffers:
		game_window->display();
		
		// Handle window events:
		game_window->dispatchEvents();
	}

	// Close the open GLFW window:
	Log::getInstance().writeMessage("Exiting main render loop...\n");
	game_window->close();

	// Dispose of shaders:
	//if (simple_shader) delete simple_shader;

	// Shutdown:
	game_window->destroy();
	delete game_window;
	deInitGLFW();

	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << "Exiting main function...\nPress enter to exit program.\n";
		std::getchar(); // wait for any character entry before exiting completely
	#endif // _DEBUG || !NDEBUG

	Log::getInstance().closeFile();
	return EXIT_SUCCESS;
}
