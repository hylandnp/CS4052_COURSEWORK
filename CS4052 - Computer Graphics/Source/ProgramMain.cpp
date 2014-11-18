/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

#if defined(_DEBUG) || !defined(NDEBUG)
	#include <iostream>
#endif // _DEBUG || NDEBUG

#include "Window.hpp"
#include "Utility/Logger.hpp"

#include "Resources/ResourceTexture2D.hpp"
#include "Resources/ResourceShader.hpp"
#include "Resources/ResourceMeshStatic.hpp"
#include "Scene/SceneNodeBase.hpp"
#include "Scene/SceneNodeTransform.hpp"
#include "Scene/SceneNodeCamera.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>


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
	Logger::getInstance().write("Entering main function.");

	// Create game window:
	if (!Window::initGLFW()) return EXIT_FAILURE;
	auto game_window = new Window();

	if (!game_window->create()) return EXIT_FAILURE;
	game_window->setActive(true);
	game_window->setVsync(true);

	// Load/setup game resources:
	ResourceTexture2D tex;
	tex.loadFromFile("Assets/barrel.jpg", true);

	ResourceShader sha,
				   sha2;

	sha.loadFromFile("Assets/phong_lighting.vert.glsl",
					 "Assets/phong_lighting.frag.glsl");
	sha2.loadFromFile("Assets/phong_lighting.vert.glsl",
					  "Assets/phong_lighting_notexture.frag.glsl");

	ResourceMeshStatic mesh,
					   stage;
	mesh.loadFromFile("Assets/barrel.obj");
	stage.loadFromFile("Assets/stage.obj");

	sha.setActive(true);
	mesh.setActive(true, true);

	SceneNodeCamera camera("Camera");
	camera.setPerspective(45.f, game_window->getWidth(), game_window->getHeight());
	camera.setPosition(0.f, 0.f, 8.f);

	SceneNodeTransform node("TestTransform", nullptr);
	node.setPosition(0.f, 0.f, 0.f);

	SceneNodeTransform node2("TestChildTransform1", nullptr);
	node2.setPosition(3.f, 0.f, 0.f);
	node2.setScale(0.5f, 0.5f, 0.5f);

	SceneNodeTransform node3("TestChildTransform2", nullptr);
	node3.setPosition(-3.f, 0.f, 0.f);
	node3.setScale(0.5f, 0.5f, 0.5f);
	
	SceneNodeTransform node4("TestTransform2", nullptr);
	node4.setPosition(-3.5f, -1.1f, 3.5f);

	SceneNodeTransform node5("TestTransform3", nullptr);
	node5.setPosition(3.5f, -1.1f, -3.5f);

	SceneNodeTransform node6("GroundTransform", nullptr);
	node6.setPosition(0.f, -3.2f, 0.f);
	node6.setRotationY(90.f);

	// Setup framerate calculation:
	double this_frame_time = glfwGetTime(),
		   last_frame_time = this_frame_time,
		   delta_time = 0.0;

	// Enter main loop:
	game_window->setVisible(true);
	while (game_window->isOpen())
	{
		this_frame_time = glfwGetTime();
		delta_time = this_frame_time - last_frame_time;
		last_frame_time = this_frame_time;

		if (!game_window->clear()) return EXIT_FAILURE;

		// Check for keyboard input:
		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_W) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_W) == GLFW_REPEAT)
		{
			camera.rotateByX(30.f * static_cast<float>(delta_time));
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_S) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_S) == GLFW_REPEAT)
		{
			camera.rotateByX(-30.f * static_cast<float>(delta_time));
		}

		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_A) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_A) == GLFW_REPEAT)
		{
			camera.rotateByY(30.f * static_cast<float>(delta_time));
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_D) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_D) == GLFW_REPEAT)
		{
			camera.rotateByY(-30.f * static_cast<float>(delta_time));
		}

		auto move_amount = 5.f * static_cast<float>(delta_time);

		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_LEFT) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_LEFT) == GLFW_REPEAT)
		{
			camera.moveByX(-move_amount);
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_RIGHT) == GLFW_PRESS ||
				 glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_RIGHT) == GLFW_REPEAT)
		{
			camera.moveByX(move_amount);
		}

		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_UP) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_DOWN) == GLFW_REPEAT)
		{
			camera.moveByZ(-move_amount);
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_DOWN) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_DOWN) == GLFW_REPEAT)
		{
			camera.moveByZ(move_amount);
		}

		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_UP) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_UP) == GLFW_REPEAT)
		{
			camera.moveByY(move_amount);
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS ||
				 glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_DOWN) == GLFW_REPEAT)
		{
			camera.moveByY(-move_amount);
		}

		// Render game scene:
		node.rotateByY(20.f * static_cast<float>(delta_time));
		node2.rotateByX(25.f * static_cast<float>(delta_time));
		node3.rotateByX(-25.f * static_cast<float>(delta_time));

		sha.setActive(true);
		sha.setUniformAttribute("view_matrix", glm::inverse(camera.getCachedGlobalMatrix()));
		sha.setUniformAttribute("proj_matrix", camera.getPerspectiveProjMatrix());
		sha.setUniformAttribute("texture_sampler", tex);

		sha2.setActive(true);
		sha2.setUniformAttribute("view_matrix", glm::inverse(camera.getCachedGlobalMatrix()));
		sha2.setUniformAttribute("proj_matrix", camera.getPerspectiveProjMatrix());

		sha.setActive(true);
		sha.setUniformAttribute("model_matrix", node.getCachedGlobalMatrix());
		mesh.setActive(true);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

		sha.setUniformAttribute("model_matrix", node.getCachedGlobalMatrix() * node2.getCachedGlobalMatrix());
		mesh.setActive(true);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

		sha.setUniformAttribute("model_matrix", node.getCachedGlobalMatrix() * node3.getCachedGlobalMatrix());
		mesh.setActive(true);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

		sha.setUniformAttribute("model_matrix", node4.getCachedGlobalMatrix());
		mesh.setActive(true);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

		sha.setUniformAttribute("model_matrix", node5.getCachedGlobalMatrix());
		mesh.setActive(true);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

		sha2.setActive(true);
		sha2.setUniformAttribute("model_matrix", node6.getCachedGlobalMatrix());
		stage.setActive(true);
		glDrawArrays(GL_TRIANGLES, 0, stage.getVertexCount());

		// Swap window buffers:
		game_window->display();
	}

	mesh.unLoad();
	tex.unLoad();
	sha.unLoad();

	// De-initialise game and dispose of managers:
	Logger::getInstance().write("Exiting main function.");
	Logger::disposeOfInstance();

	// Close the game window:
	game_window->close();
	delete game_window;
	Window::deInitGLFW();

	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << "Press ENTER to exit program...\n";
		std::getchar();
	#endif // _DEBUG || NDEBUG

	return EXIT_SUCCESS;
}
