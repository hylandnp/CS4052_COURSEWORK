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



	// LAB 0 stuff:
	GLfloat triangle_points[] =
	{
		0.8f, 0.8f, 0.f, // triangle 1
		0.8f, -0.8f, 0.f,
		-0.8f, -0.8f, 0.f,
		-0.8f, -0.8f, 0.f, // triange 2
		-0.8f, 0.8f, 0.f,
		0.8f, 0.8f, 0.f
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

	// LAB 1 stuff:
	Camera* game_camera = new Camera();
	game_camera->setPerspective(45.f,
								static_cast<float>(game_window->getWidth()) / static_cast<float>(game_window->getHeight()),
								0.1f,
								300.f);
	game_camera->lookAt(glm::vec3(0.f, 0.f, 3.f),
						glm::vec3(0.f, 0.f, 0.f));

	// Transformations for multiple primitives:
	std::vector<Transform> transforms;

	// Transformation flags:
	bool rotate_test = false,
	     move_test = false,
		 uniform_scale_test = false,
		 scale_x_test = false,
		 scale_y_test = false,
		 scale_z_test = false,
		 all_test = false,
		 grow_not_shrink = true;
	
	transforms.push_back(Transform());
	transforms[0].Location = glm::vec3(-0.8f, -0.4f, 0.f);
	transforms[0].RotationAxes = glm::vec3(1.f, 0.f, 0.f);
	transforms[0].RotationValue = 0.f;
	transforms[0].Scale = glm::vec3(0.5f, 0.5f, 0.5f);

	transforms.push_back(Transform());
	transforms[1].Location = glm::vec3(0.8f, -0.4f, 0.f);
	transforms[1].RotationAxes = glm::vec3(0.f, 1.f, 0.f);
	transforms[1].RotationValue = 0.f;
	transforms[1].Scale = glm::vec3(0.5f, 0.5f, 0.5f);

	transforms.push_back(Transform());
	transforms[2].Location = glm::vec3(0.f, 0.5f, 0.f);
	transforms[2].RotationAxes = glm::vec3(0.f, 0.f, 1.f);
	transforms[2].RotationValue = 0.f;
	transforms[2].Scale = glm::vec3(0.5f, 0.5f, 0.5f);

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

		// Handle input:
		if (Input::getInstance().isKeyReleased(GLFW_KEY_R))
		{
			rotate_test = !rotate_test;
		}

		if (Input::getInstance().isKeyReleased(GLFW_KEY_A))
		{
			all_test = !all_test;
		}
		
		if (Input::getInstance().isKeyReleased(GLFW_KEY_U))
		{
			uniform_scale_test = !uniform_scale_test;
			scale_x_test = false;
			scale_y_test = false;
			scale_z_test = false;
		}

		if (Input::getInstance().isKeyReleased(GLFW_KEY_X))
		{
			scale_x_test = !scale_x_test;
			uniform_scale_test = false;
			scale_y_test = false;
			scale_z_test = false;
		}

		if (Input::getInstance().isKeyReleased(GLFW_KEY_Y))
		{
			scale_y_test = !scale_y_test;
			uniform_scale_test = false;
			scale_x_test = false;
			scale_z_test = false;
		}

		if (Input::getInstance().isKeyReleased(GLFW_KEY_Z))
		{
			scale_z_test = !scale_z_test;
			uniform_scale_test = false;
			scale_x_test = false;
			scale_y_test = false;
		}

		// LAB 1 stuff:
		for (auto& elem : transforms)
		{
			simple_shader->asActiveShader();

			// Transform based on input:
			if (rotate_test || all_test)
			{
				elem.RotationValue += 40.f * static_cast<float>(delta_time);
				if (elem.RotationValue >= 360.f) elem.RotationValue = 0.f;
			}
			else
			{
				elem.RotationValue = 0.f;
			}

			if (uniform_scale_test || all_test)
			{
				elem.Scale += ((grow_not_shrink) ?
							   glm::vec3(0.5f * static_cast<float>(delta_time))	:
							   glm::vec3(-0.5f * static_cast<float>(delta_time)));
			}
			else if (scale_x_test)
			{
				elem.Scale.x += ((grow_not_shrink) ?
								 0.5f * static_cast<float>(delta_time) :
								 -0.5f * static_cast<float>(delta_time));
			}
			else if (scale_y_test)
			{
				elem.Scale.y += ((grow_not_shrink) ?
							  	 0.5f * static_cast<float>(delta_time) :
								 -0.5f * static_cast<float>(delta_time));
			}
			else if (scale_z_test)
			{
				elem.Scale.z += ((grow_not_shrink) ?
								 0.5f * static_cast<float>(delta_time) :
								 -0.5f * static_cast<float>(delta_time));
			}
			else
			{
				elem.Scale = glm::vec3(0.5f, 0.5f, 0.5f);
			}

			if ((elem.Scale.x < 0.2f || elem.Scale.x > 0.8f) ||
				(elem.Scale.y < 0.2f || elem.Scale.y > 0.8f) ||
				(elem.Scale.z < 0.2f || elem.Scale.z > 0.8f))
			{
				grow_not_shrink = !grow_not_shrink;
			}

			if (Input::getInstance().isKeyPressed(GLFW_KEY_LEFT))
			{
				elem.Location.x -= 0.5f * static_cast<float>(delta_time);
			}
			else if (Input::getInstance().isKeyPressed(GLFW_KEY_RIGHT))
			{
				elem.Location.x += 0.5f * static_cast<float>(delta_time);
			}

			if (Input::getInstance().isKeyPressed(GLFW_KEY_DOWN))
			{
				elem.Location.y -= 0.5f * static_cast<float>(delta_time);
			}
			else if (Input::getInstance().isKeyPressed(GLFW_KEY_UP))
			{
				elem.Location.y += 0.5f * static_cast<float>(delta_time);
			}

			if (Input::getInstance().isKeyPressed(GLFW_KEY_N))
			{
				elem.Location.z -= 0.5f * static_cast<float>(delta_time);
			}
			else if (Input::getInstance().isKeyPressed(GLFW_KEY_M))
			{
				elem.Location.z += 0.5f * static_cast<float>(delta_time);
			}

			elem.rebuildMatrix();
			simple_shader->setAttribute("transformation", game_camera->getTransform(elem.TranslationMatrix));

			glBindVertexArray(triangle_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// Swap the window buffers:
		game_window->display();
		
		// Handle window events:
		game_window->dispatchEvents();
	}

	Log::getInstance().writeMessage("Exiting main render loop...\n");
	game_window->close();
	

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
