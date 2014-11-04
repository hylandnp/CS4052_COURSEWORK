/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include <GLFW/glfw3.h>
#include <cstdlib>

#if defined(_DEBUG) || !defined(NDEBUG)
	#include <iostream>
#endif // _DEBUG || NDEBUG

#include "Utility/Logger.hpp"


// Program entry point function:
#if (defined(_WIN32) || defined(WIN32)) && !defined(_CONSOLE)
	#include <windows.h>

	// If application build type is Win32 GUI or any other non-console type, use the Windows entry point function:
	int WINAPI WinMain(HINSTANCE p_curr_inst, HINSTANCE p_prev_inst, LPSTR p_cmd_line, int p_cmd_show)
#else
	// Otherwise use regular entry point function:
	int main(int p_argc, char** p_argv)
#endif
{
	// Initialise game and create managers:
	Logger::forceCreateInstance();

	// Setup file logger:
	if (!Logger::getInstance().openFile("cs4052.log")) return EXIT_FAILURE;

	// Setup framerate calculation:
	double last_frame_time = glfwGetTime(),
		   this_frame_time = 0.0,
		   delta_time = 0.0;

	// Enter main loop:
	bool running = true;

	//while (running)
	{
		// TODO
	}


	// De-initialise game and dispose of managers:
	Logger::disposeOfInstance();

	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << "Press ENTER to exit program...\n";
		std::getchar();
	#endif // _DEBUG || NDEBUG

	return EXIT_SUCCESS;
}
