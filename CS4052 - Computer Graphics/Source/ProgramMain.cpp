/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Program entry point ('main' function).
*/
#include "Window.hpp"
#include "OpenGL.hpp"

#include <iostream>


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
	std::cout << "Entering main function...\n";
	if (!initGLFW()) return EXIT_FAILURE;

	//// Startup:
	//Window game_window* = new Window();
	//if (!game_window->create()) return EXIT_FAILURE;
	//
	//// TODO

	//// Runtime:
	//game_window->setVisible();
	//
	//while (game_window->isOpen())
	//{
	//	// Clear the window:
	//	if (!game_window->clear()) return EXIT_FAILURE;

	//	// Render the game scene:
	//	// TODO

	//	// Swap the window buffers:
	//	game_window->display();
	//	
	//	// Handle window events:
	//	game_window->dispatchEvents();
	//}
	//
	//// Shutdown:
	//game_window->destroy();
	//delete game_window;
	//
	//// TODO

	deInitGLFW();
	std::cout << "Exiting main function...\n";

	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << "\nPress enter to exit program.\n";
		std::getchar(); // wait for any character entry before exiting completely
	#endif // _DEBUG || !NDEBUG

	return EXIT_SUCCESS;
}


