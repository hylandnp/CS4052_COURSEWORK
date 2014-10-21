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

	// Load meshes:
	GLfloat cube_vertices[] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	GLfloat cube_colours[] =
	{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};

	std::size_t cube_vertex_count = 36;

	GLuint cube_vbo = 0,
		   cube_vao = 0,
		   colour_vbo = 0;

	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &colour_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colours), cube_colours, GL_STATIC_DRAW);

	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// Setup transformations:
	Transform cube_trans;
	cube_trans.setRotationAxes(glm::vec3(1.f, 1.f, 1.f));

	// Create camera:
	Camera* game_camera = new Camera();
	game_camera->lookAt(glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f, 0.f, 0.f));

	// Setup rotation values:
	float rval = 0.f,
		  rval2 = 0.f;

	// Begin runtime:
	game_window->setVisible();
	Log::getInstance().writeMessage("Entering main render loop...\n");

	// Setup framerate calculations:
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

		// Update the model rotations:
		rval += 100.f * static_cast<float>(delta_time);
		rval2 -= 100.f * static_cast<float>(delta_time);
		if (rval >= 360.f) rval = 0.f;
		if (rval2 <= 0.f) rval2 = 360.f;

		// Render the game scene:
		simple_shader->asActiveShader();

		// Top-left viewport:
		game_camera->setViewport(0,
								 game_window->getHeight() / 2,
								 game_window->getWidth() / 2,
								 game_window->getHeight() / 2);
		game_camera->setOrtho(0.f,
							  game_window->getWidth() / 2.f,
							  game_window->getHeight() / 2.f,
							  0.f);
		game_camera->lookAt(glm::vec3(0.f, 0.f, 2.f),
							glm::vec3(0.f, 0.f, 0.f));
		
		cube_trans.setRotationValue(0.f);
		cube_trans.setScale(glm::vec3(100.f, 100.f, 100.f));
		cube_trans.setLocation(glm::vec3(200.f, 150.f, 0.f));

		simple_shader->setAttribute("transformation", game_camera->transform(cube_trans.getMatrix()));

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count);

		// Top-right viewport:
		game_camera->setViewport(game_window->getWidth() / 2,
								 game_window->getHeight() / 2,
								 game_window->getWidth() / 2,
								 game_window->getHeight() / 2);
		game_camera->setPerspective(45.f,
									static_cast<float>(game_window->getWidth()),
									static_cast<float>(game_window->getHeight()));
		game_camera->lookAt(glm::vec3(0.f, 0.f, 2.f),
							glm::vec3(0.f, 0.f, 0.f));
		
		cube_trans.setRotationValue(rval);
		cube_trans.setScale(glm::vec3(1.f, 1.f, 1.f));
		cube_trans.setLocation(glm::vec3(0.f, 0.f, -2.f));

		simple_shader->setAttribute("transformation", game_camera->transform(cube_trans.getMatrix()));

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count);

		// Bottom-left viewport:
		game_camera->setViewport(0,
								 0,
								 game_window->getWidth() / 2,
								 game_window->getHeight() / 2);
		game_camera->setPerspective(90.f,
									static_cast<float>(game_window->getWidth()),
									static_cast<float>(game_window->getHeight()));
		game_camera->lookAt(glm::vec3(2.f, 2.f, 1.f),
							glm::vec3(0.f, 0.f, 0.f));

		cube_trans.setRotationValue(rval2);
		cube_trans.setScale(glm::vec3(1.f, 1.f, 1.f));
		cube_trans.setLocation(glm::vec3(0.f, 0.f, -2.f));

		simple_shader->setAttribute("transformation", game_camera->transform(cube_trans.getMatrix()));

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count);

		// Bottom-right viewport:
		game_camera->setViewport(game_window->getWidth() / 2,
								 0,
								 game_window->getWidth() / 2,
								 game_window->getHeight() / 2);
		game_camera->setPerspective(30.f,
									static_cast<float>(game_window->getWidth()),
									static_cast<float>(game_window->getHeight()));
		game_camera->lookAt(glm::vec3(0.f, 0.f, 2.5f),
							glm::vec3(0.f, 0.f, 0.f));

		cube_trans.setRotationValue(0.f);
		cube_trans.setScale(glm::vec3(1.f, 1.f, 1.f));
		cube_trans.setLocation(glm::vec3(0.f, 0.f, 0.f));

		simple_shader->setAttribute("transformation", game_camera->transform(cube_trans.getMatrix()));

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count);

		// Swap the window buffers:
		game_window->display();
		
		// Handle window events:
		game_window->dispatchEvents();
	}

	// Close the open GLFW window:
	Log::getInstance().writeMessage("Exiting main render loop...\n");
	game_window->close();

	// Dispose of the camera:
	if (game_camera) delete game_camera;

	// Dispose of shaders:
	if (simple_shader) delete simple_shader;

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
