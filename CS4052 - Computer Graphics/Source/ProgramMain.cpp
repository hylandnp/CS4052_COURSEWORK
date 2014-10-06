/*
CS4051 - Computer Graphics
NEIL HYLAND (11511677)

Program entry point ('main' function).
*/
#include "OpenGL.hpp"
#include "Log.hpp"
#include "Window.hpp"
#include "Shader.hpp"

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
	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << "Entering main function...\n";
	#endif // _DEBUG || !NDEBUG

	// Startup:
	Log::getInstance().openFile("cs4051.log");

	if (!initGLFW()) return EXIT_FAILURE;
	Window* game_window = new Window();

	if (!game_window->create(600, 600))
	{
		delete game_window;
		return EXIT_FAILURE;
	}

	game_window->asActiveContext();


	// LAB 0 stuff:
	GLfloat triangle_points[] =
	{
		0.5f, 0.5f, 0.f, // triangle 1
		0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f, // triange 2
		-0.5f, 0.5f, 0.f,
		0.5f, 0.5f, 0.f
	};

	GLfloat triangle_colours[] =
	{
		1.f, 0.f, 0.f,
		0.7f, 1.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		0.7f, 1.f, 0.f,
		1.f, 0.f, 0.f
	};

	GLuint triangle_vbo = 0,
		   triangle_vao = 0,
		   colour_vbo = 0;

	glGenBuffers(1, &triangle_vbo);
	glGenBuffers(1, &colour_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(triangle_points),
				 triangle_points,
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
	glBufferData(GL_ARRAY_BUFFER,
				 sizeof(triangle_colours),
				 triangle_colours,
				 GL_STATIC_DRAW);

	// Create VAO:
	glGenVertexArrays(1, &triangle_vao);

	// Vertex attributes:
	glBindVertexArray(triangle_vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Colour attributes:
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Shader:
	Shader* simple_shader = new Shader();

	if (!simple_shader->loadFromFile("Assets/SimpleShader.vert.glsl",
									 "Assets/SimpleShader.frag.glsl"))
	{
		return EXIT_FAILURE;
	}



	// Runtime:
	game_window->setVisible();
	Log::getInstance().writeMessage("Entering main render loop...\n");
	
	while (game_window->isOpen())
	{
		// Clear the window:
		if (!game_window->clear()) return EXIT_FAILURE;

		// Render the game scene:
		// TODO

		// LAB 0 stuff:
		simple_shader->asActiveShader();
		glBindVertexArray(triangle_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// Swap the window buffers:
		game_window->display();
		
		// Handle window events:
		game_window->dispatchEvents();
	}

	Log::getInstance().writeMessage("Exiting main render loop...\n");
	

	// LAB 0 stuff:
	simple_shader->unLoad();
	delete simple_shader;




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
