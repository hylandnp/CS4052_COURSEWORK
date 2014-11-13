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
	game_window->setVsync(false);



	ResourceTexture2D tex;
	tex.loadFromFile("Assets/barrel.jpg", true);

	ResourceShader sha;

	sha.loadFromFile("Assets/phong_lighting.vert.glsl",
					 "Assets/phong_lighting.frag.glsl");

	ResourceMeshStatic mesh;
	mesh.loadFromFile("Assets/barrel.obj");

	sha.setActive(true);
	mesh.setActive(true, true);

	glm::mat4 proj = glm::perspective(glm::radians(45.f), game_window->getWidth() / (float)game_window->getHeight(), 0.1f, 300.f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 8.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));

	SceneNodeCamera camera("Camera");
	camera.setPerspective(45.f, game_window->getWidth(), game_window->getHeight());
	camera.setPosition(0.f, 0.f, -8.f);
	//camera.lookAt(glm::vec3(0.f));

	SceneNodeTransform node("TestTransform", nullptr);
	node.setPosition(0.f, 0.f, 0.f);

	//std::cout << (int)(view == camera.getCachedGlobalMatrix()) << std::endl;

	//SceneNodeTransform node("Root", nullptr);
	//node.addChild(new SceneNodeTransform("Child1", nullptr));
	//node.addChild(new SceneNodeTransform("Child2", nullptr));
	//node.getChild("Child2")->addChild(new SceneNodeTransform("Child3", nullptr));
	//node.getChild("Child2")->addChild(new SceneNodeTransform("Child4", nullptr));

	//std::cout << node.countChildren(true) << std::endl;
	//std::cout << node.getTreeInfo() << std::endl;

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
		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_LEFT) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_LEFT) == GLFW_REPEAT)
		{
			camera.moveByX(5.f * static_cast<float>(delta_time));
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_RIGHT) == GLFW_PRESS ||
				 glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_RIGHT) == GLFW_REPEAT)
		{
			camera.moveByX(-5.f * static_cast<float>(delta_time));
		}

		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_UP) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_DOWN) == GLFW_REPEAT)
		{
			camera.moveByZ(5.f * static_cast<float>(delta_time));
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_DOWN) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_DOWN) == GLFW_REPEAT)
		{
			camera.moveByZ(-5.f * static_cast<float>(delta_time));
		}

		if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_UP) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_UP) == GLFW_REPEAT)
		{
			camera.moveByY(-5.f * static_cast<float>(delta_time));
		}
		else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS ||
			glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_PAGE_DOWN) == GLFW_REPEAT)
		{
			camera.moveByY(5.f * static_cast<float>(delta_time));
		}

		// Render game scene:
		// TODO

		//node.rotateByY(20.f * static_cast<float>(delta_time));
		//camera.moveByX(0.2f * (float)delta_time);
		//node.setRotationAsQuaternion(glm::quat(glm::radians(node.getRotationInEulerAngles() + glm::vec3(0.f, 100.f * (float)delta_time, 0.f))));
		//std::cout << node.getRotationInEulerAngles().y << std::endl;

		sha.setUniformAttribute("model_matrix", node.getCachedGlobalMatrix());
		sha.setUniformAttribute("view_matrix", camera.getCachedGlobalMatrix());
		sha.setUniformAttribute("proj_matrix", camera.getPerspectiveProjMatrix());
		sha.setUniformAttribute("texture_sampler", tex);

		mesh.setActive(true);
		glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

		game_window->display();
	}

	mesh.unLoad();
	tex.unLoad();
	sha.unLoad();
	//node.removeAllChildren();

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
