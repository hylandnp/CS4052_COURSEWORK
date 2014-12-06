/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

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

	// Setup physics:
	auto bullet_broadphase = new btDbvtBroadphase();
	auto bullet_collision_config = new btDefaultCollisionConfiguration();
	auto bullet_collision_dispatcher = new btCollisionDispatcher(bullet_collision_config);
	auto bullet_solver = new btSequentialImpulseConstraintSolver();
	auto bullet_dynamics_world = new btDiscreteDynamicsWorld(bullet_collision_dispatcher,
															 bullet_broadphase,
															 bullet_solver,
															 bullet_collision_config);
	bullet_dynamics_world->setGravity(btVector3(0.f, -10.f, 0.f));

	// Load/setup game resources:
	ResourceTexture2D tex;
	tex.loadFromFile("Assets/barrel.jpg", true);

	ResourceShader sha,
				   sha2;

	sha.loadFromFile("Assets/phong_lighting.vert.glsl",
					 "Assets/phong_lighting.frag.glsl");
	sha2.loadFromFile("Assets/phong_lighting.vert.glsl",
					  "Assets/phong_lighting_notexture.frag.glsl");

	ResourceMeshStatic barrel_mesh,
					   launch_ramp_mesh;
	barrel_mesh.loadFromFile("Assets/barrel2.obj");
	launch_ramp_mesh.loadFromFile("Assets/launch_ramp.obj");

	sha.setActive(true);
	barrel_mesh.setActive(true, true);

	SceneNodeCamera camera("Camera");
	camera.setPerspective(45.f, game_window->getWidth(), game_window->getHeight());
	camera.setPosition(0.f, 2.5f, 4.f);
	//camera.setRotationX(-45.f);

	SceneNodeTransform barrel_transform("TestTransform", nullptr);
	barrel_transform.setPosition(0.f, 1.5f, 1.4f); // y = 0.9f

	SceneNodeTransform launch_ramp_transform("TestChildTransform1", nullptr);
	launch_ramp_transform.setPosition(0.f, 0.f, 0.f);

	// Setup framerate calculation:
	double this_frame_time = glfwGetTime(),
		   last_frame_time = this_frame_time,
		   delta_time = 0.0;

	// Setup physics objects:
	auto ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	// Setup physics scene motion states:
	auto ground_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	auto fall_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3, 1)));
	auto ramp_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));

	// Setup ground collision plane:
	auto ground_rigid_body = new btRigidBody(0,
											 ground_motion_state,
											 ground_shape,
											 btVector3(0, 0, 0));
	bullet_dynamics_world->addRigidBody(ground_rigid_body);

	// Setup ramp collision body:
	btVector3 launch_ramp_local_inertia;
	launch_ramp_mesh.getCollisionMeshObject()->calculateLocalInertia(0, launch_ramp_local_inertia);

	auto launch_ramp_rigid_body = new btRigidBody(0,
												  ramp_motion_state,
												  launch_ramp_mesh.getCollisionMeshObject(),
												  launch_ramp_local_inertia);
	bullet_dynamics_world->addRigidBody(launch_ramp_rigid_body);

	// Setup barrel rigid/collision body:
	btScalar barrel_mass = 10;
	btVector3 barrel_local_inertia;
	barrel_mesh.getCollisionMeshObject()->calculateLocalInertia(barrel_mass, barrel_local_inertia);

	auto barrel_rigid_body = new btRigidBody(barrel_mass,
											 fall_motion_state,
											 barrel_mesh.getCollisionMeshObject(),
											 barrel_local_inertia);
	barrel_rigid_body->setRestitution(1);
	bullet_dynamics_world->addRigidBody(barrel_rigid_body);

	// Glue transformation matrix between OpenGL/GLM and Bullet:
	glm::mat4 ATTRIBUTE_ALIGNED16(aligned_model_matrix);

	// Enter main loop:
	game_window->setVisible(true);
	while (game_window->isOpen())
	{
		this_frame_time = glfwGetTime();
		delta_time = this_frame_time - last_frame_time;
		last_frame_time = this_frame_time;

		if (!game_window->clear()) return EXIT_FAILURE;

		// Update physics step:
		bullet_dynamics_world->stepSimulation(static_cast<float>(delta_time));

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

		//if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_A) == GLFW_PRESS ||
		//	glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_A) == GLFW_REPEAT)
		//{
		//	camera.rotateByY(30.f * static_cast<float>(delta_time));
		//}
		//else if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_D) == GLFW_PRESS ||
		//	glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_D) == GLFW_REPEAT)
		//{
		//	camera.rotateByY(-30.f * static_cast<float>(delta_time));
		//}

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

		// Pass camera matrices to shader(s):
		sha.setActive(true);
		sha.setUniformAttribute("view_matrix", glm::inverse(camera.getCachedGlobalMatrix()));
		sha.setUniformAttribute("proj_matrix", camera.getPerspectiveProjMatrix());
		sha.setUniformAttribute("light_position_world", camera.getPosition() + glm::vec3(0.f, 4.f, 0.f));

		sha2.setActive(true);
		sha2.setUniformAttribute("view_matrix", glm::inverse(camera.getCachedGlobalMatrix()));
		sha2.setUniformAttribute("proj_matrix", camera.getPerspectiveProjMatrix());
		sha2.setUniformAttribute("light_position_world", camera.getPosition() + glm::vec3(0.f, 4.f, 0.f));

		// Update the barrel transform:
		barrel_rigid_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(aligned_model_matrix));

		// Draw the barrel:
		sha.setActive(true);
		sha.setUniformAttribute("model_matrix", aligned_model_matrix); // barrel_transform.getCachedGlobalMatrix());
		barrel_mesh.setActive(true, true);
		glDrawArrays(GL_TRIANGLES, 0, barrel_mesh.getVertexCount());

		// Update the ramp transform:
		launch_ramp_rigid_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(aligned_model_matrix));

		// Draw the launch ramp:
		sha2.setActive(true);
		sha2.setUniformAttribute("model_matrix", aligned_model_matrix);
		launch_ramp_mesh.setActive(true, true);
		glDrawArrays(GL_TRIANGLES, 0, launch_ramp_mesh.getVertexCount());

		// Swap window buffers:
		game_window->display();
	}

	barrel_mesh.unLoad();
	tex.unLoad();
	sha.unLoad();

	// De-initialise game and dispose of managers:
	Logger::getInstance().write("Exiting main function.");
	Logger::disposeOfInstance();

	// Close the game window:
	game_window->close();
	delete game_window;
	Window::deInitGLFW();

	// Close physics:
	delete bullet_dynamics_world;
	delete bullet_solver;
	delete bullet_collision_dispatcher;
	delete bullet_collision_config;
	delete bullet_broadphase;

	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << "Press ENTER to exit program...\n";
		std::getchar();
	#endif // _DEBUG || NDEBUG

	return EXIT_SUCCESS;
}
