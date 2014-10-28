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

#include "meshloader.hpp"
#include "imageloader.hpp"


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
	if (!simple_shader->loadFromFile("Assets/Shader.vert.glsl",
									 "Assets/Shader.frag.glsl"))
	{
		return EXIT_FAILURE;
	}

	// Load meshes:
	GLuint barrel_vbo = 0,
		   barrel_vao = 0,
		   vertex_count;

	vertex_count = loadMesh("Assets/barrel.obj", &barrel_vao, &barrel_vbo);

	// Load textures:
	auto texture_id = loadTexture("Assets/barrel.jpg");
	if (texture_id == 0) return EXIT_FAILURE;

	// Setup transformations:
	Transform obj_trans;
	obj_trans.setRotationAxes(glm::vec3(1.f, 1.f, 1.f));

	// Create camera:
	Camera* game_camera = new Camera();
	game_camera->setPerspective(45.f,
								static_cast<float>(game_window->getWidth()),
								static_cast<float>(game_window->getHeight()));
	game_camera->setViewport(0, 0, game_window->getWidth(), game_window->getHeight());
	game_camera->lookAt(glm::vec3(0.f, 0.f, 6.f), glm::vec3(0.f, 0.f, 0.f));

	// Setup rotation values:
	float rval = 0.f;

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
		if (rval >= 360.f) rval = 0.f;
		obj_trans.setRotationValue(rval);

		simple_shader->asActiveShader();
		glBindTexture(GL_TEXTURE_2D, texture_id);
		simple_shader->setAttribute("mvp_matrix", game_camera->transform(obj_trans.getMatrix()));

		auto img_loc = glGetUniformLocation(simple_shader->getRawProgramHandle(), "texture_sampler");
		glUniform1i(img_loc, 0);

		glBindVertexArray(barrel_vao);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

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

	// Dispose of meshes:
	//

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
