/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
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

	// Setup framerate calculation:
	double last_frame_time = glfwGetTime(),
		   this_frame_time = 0.0,
		   delta_time = 0.0;



	ResourceTexture2D tex;
	tex.loadFromFile("Assets/barrel.jpg", true);

	ResourceShader sha;
	sha.loadFromFile("Assets/default_texture_nolighting.vert.glsl",
					 "Assets/default_texture_nolighting.frag.glsl");

	ResourceMeshStatic mesh;
	mesh.loadFromFile("Assets/barrel.obj");

	sha.setActive(true);
	sha.setUniformAttribute("texture_sampler", tex);
	mesh.setActive(true, true);

	SceneNodeBase node("Root", SceneNodeType::INVALID, nullptr);
	node.addChild(new SceneNodeBase("Child1", SceneNodeType::INVALID, nullptr));
	node.addChild(new SceneNodeBase("Child2", SceneNodeType::INVALID, nullptr));
	node.getChild("Child2")->addChild(new SceneNodeBase("Child3", SceneNodeType::INVALID, nullptr));
	node.getChild("Child2")->addChild(new SceneNodeBase("Child4", SceneNodeType::INVALID, nullptr));

	std::cout << node.countChildren(true) << std::endl;
	std::cout << node.getTreeInfo() << std::endl;

	// Enter main loop:
	game_window->setVisible(true);
	while (game_window->isOpen())
	{
		if (!game_window->clear()) return EXIT_FAILURE;

		// Render game scene:
		// TODO

		game_window->display();
	}

	mesh.unLoad();
	tex.unLoad();
	sha.unLoad();
	node.removeAllChildren();

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
