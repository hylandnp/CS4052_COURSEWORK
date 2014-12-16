/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <string>
#include <sstream>

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

#include "text.h"

#define GAME_MENU 1
#define GAME_PLAY 2
#define GAME_OVER 3



// Collision callback for wall-barrel-ground contact:
struct WallBarrelCollisionResultCallback : public btCollisionWorld::ContactResultCallback
{
	btScalar addSingleResult(btManifoldPoint& cp,
							 const btCollisionObjectWrapper* colObj0Wrap,
							 int partId0,
							 int index0,
							 const btCollisionObjectWrapper* colObj1Wrap,
							 int partId1,
							 int index1)
	{
		has_hit = false;

		if (colObj0Wrap->m_collisionObject == barrel_ptr || colObj1Wrap->m_collisionObject == barrel_ptr)
		{
			has_hit = true;
		}

		return 0;
	}

	btRigidBody* barrel_ptr;
	btRigidBody* target_ptr;

	bool has_hit = false;
};




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

	// Init text rendering:
	init_text_rendering("Assets/freemono.png",
						"Assets/freemono.meta",
						game_window->getWidth(),
						game_window->getHeight());
	int score_text = add_text("Current Score: ", -0.95f, 1.f, 60, 1.f, 1.f, 1.f, 1.f),
		barrel_text = add_text("Barrels Remaining: ", -0.95f, 0.9f, 60, 1.f, 1.f, 1.f, 1.f),
		play_text = add_text("Play Game", 0.f, 0.3f, 90, 1.f, 1.f, 1.f, 1.f),
		quit_text = add_text("Quit Game", 0.f, -0.1f, 90, 1.f, 1.f, 1.f, 1.f),
		over_text = add_text("SHOOTING FISH WITH A BARREL", -0.8f, 0.5f, 90, 1.f, 1.f, 1.f, 1.f);

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
	ResourceTexture2D tex,
					  tex2;
	tex.loadFromFile("Assets/barrel.jpg", true);
	tex2.loadFromFile("Assets/fish_wall.png", true);

	ResourceShader sha,
				   sha2;

	sha.loadFromFile("Assets/phong_lighting.vert.glsl",
					 "Assets/phong_lighting.frag.glsl");
	sha2.loadFromFile("Assets/phong_lighting.vert.glsl",
					  "Assets/phong_lighting_notexture.frag.glsl");

	ResourceMeshStatic barrel_mesh,
					   launch_ramp_mesh,
					   wall_mesh,
					   cog_mesh,
					   pole_mesh;
	barrel_mesh.loadFromFile("Assets/barrel2.obj", false);
	launch_ramp_mesh.loadFromFile("Assets/launch_ramp.obj", true);
	wall_mesh.loadFromFile("Assets/fish_wall2.obj", true);
	cog_mesh.loadFromFile("Assets/cog.obj", true);
	pole_mesh.loadFromFile("Assets/pole.obj", true);

	sha.setActive(true);
	barrel_mesh.setActive(true, true);

	SceneNodeTransform barrel_icon_trans;
	barrel_icon_trans.setScale(0.5f);
	float barrel_rotation_val = 30.f;
	bool play_hover = true;

	glm::vec3 play_pos(-0.5f, 2.9f, -2.f),
			  quit_pos(-0.5f, 1.9f, -2.f);

	barrel_icon_trans.setPosition(play_pos);

	// Animated cog transforms:
	SceneNodeTransform cog1,
					   cog2,
					   cog3,
					   cog4,
					   cog5,
					   cog6;
	cog1.setPosition(-1.3f, 2.1f, 0.f);
	cog2.setPosition(1.3f, 2.1f, 0.f);

	cog3.setPosition(-0.3f, 0.6f, 0.f);
	cog4.setPosition(0.3f, -0.6f, 0.f);
	cog3.setScale(0.3f);
	cog4.setScale(0.3f);

	cog5.setPosition(-1.2f, 1.4f, 1.7f);
	cog6.setPosition(1.2f, 1.4f, 1.7f);
	cog5.setScale(0.5f);
	cog6.setScale(0.5f);

	float cog_rotation = 20.f;

	SceneNodeCamera camera("Camera");
	camera.setPerspective(45.f, game_window->getWidth(), game_window->getHeight());
	camera.setPosition(0.f, 3.f, 4.f);
	camera.setRotationX(-5.f);

	// Setup framerate calculation:
	double this_frame_time = glfwGetTime(),
		   last_frame_time = this_frame_time,
		   delta_time = 0.0;

	// Setup physics objects:
	auto ground_shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	// Setup physics scene motion states:
	auto ground_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	auto fall_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2.2f, 1)));
	auto ramp_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));
	auto wall_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, -35)));
	auto pole_motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, -20)));

	// Setup ground collision plane:
	auto ground_rigid_body = new btRigidBody(0,
											 ground_motion_state,
											 ground_shape,
											 btVector3(0, 0, 0));
	bullet_dynamics_world->addRigidBody(ground_rigid_body);

	// Setup ramp collision body:
	auto launch_ramp_rigid_body = new btRigidBody(0,
												  ramp_motion_state,
												  launch_ramp_mesh.getCollisionMeshObject(),
												  btVector3(0, 0, 0));
	bullet_dynamics_world->addRigidBody(launch_ramp_rigid_body);

	// Setup wall collision body:
	auto wall_rigid_body = new btRigidBody(0,
										   wall_motion_state,
										   wall_mesh.getCollisionMeshObject(),
										   btVector3(0, 0, 0));
	bullet_dynamics_world->addRigidBody(wall_rigid_body);

	// Setup pole collision body:
	auto pole_rigid_body = new btRigidBody(0,
										   pole_motion_state,
										   pole_mesh.getCollisionMeshObject(),
										   btVector3(0, 0, 0));
	bullet_dynamics_world->addRigidBody(pole_rigid_body);

	// Setup barrel rigid/collision body:
	btScalar barrel_mass = 10;
	btVector3 barrel_local_inertia;
	barrel_mesh.getCollisionMeshObject()->calculateLocalInertia(barrel_mass, barrel_local_inertia);

	auto barrel_rigid_body = new btRigidBody(barrel_mass,
											 fall_motion_state,
											 barrel_mesh.getCollisionMeshObject(),
											 barrel_local_inertia);
	barrel_rigid_body->setRestitution(0.8f);
	bullet_dynamics_world->addRigidBody(barrel_rigid_body);

	// Glue transformation matrix between OpenGL/GLM and Bullet:
	glm::mat4 ATTRIBUTE_ALIGNED16(aligned_model_matrix);

	// Setup launch parameters:
	btVector3 relative_force(0, 0, 1000),
			  corrected_force(0, 0, 0),
			  zero_vector(0.f, 0.f, 0.f);
	btTransform launch_trans;
	bool is_launched = false;

	// Setup callback:
	WallBarrelCollisionResultCallback collision_callback;
	collision_callback.barrel_ptr = barrel_rigid_body;
	collision_callback.target_ptr = wall_rigid_body;

	// Setup pole movement:
	bool pole_move_left = true;

	// Game states:
	char current_game_state = GAME_MENU;
	int remaining_lives = 3,
		score = 0;
	std::stringstream str_stream;

	float input_timeout = 0.4f,
		  input_last_time = 0.f,
		  collision_last_time = 0.f,
		  launch_timeout = 1.f;

	// Enter main loop:
	game_window->setVisible(true);
	while (game_window->isOpen())
	{
		this_frame_time = glfwGetTime();
		delta_time = this_frame_time - last_frame_time;
		last_frame_time = this_frame_time;

		if (!game_window->clear()) return EXIT_FAILURE;

		// Pass camera matrices to shader(s):
		sha.setActive(true);
		sha.setUniformAttribute("view_matrix", glm::inverse(camera.getCachedGlobalMatrix()));
		sha.setUniformAttribute("proj_matrix", camera.getPerspectiveProjMatrix());
		sha.setUniformAttribute("light_position_world", camera.getPosition() + glm::vec3(0.f, 4.f, 0.f));

		sha2.setActive(true);
		sha2.setUniformAttribute("view_matrix", glm::inverse(camera.getCachedGlobalMatrix()));
		sha2.setUniformAttribute("proj_matrix", camera.getPerspectiveProjMatrix());
		sha2.setUniformAttribute("light_position_world", camera.getPosition() + glm::vec3(0.f, 4.f, 0.f));

		if (current_game_state == GAME_PLAY)
		{
			// Update cog rotations:
			cog1.rotateByX(cog_rotation * static_cast<float>(delta_time));
			cog2.rotateByX(cog_rotation * static_cast<float>(delta_time));
			cog3.rotateByX(-cog_rotation * 2 * static_cast<float>(delta_time));
			cog4.rotateByX(-cog_rotation * 2 * static_cast<float>(delta_time));
			cog5.rotateByX(-cog_rotation * static_cast<float>(delta_time));
			cog6.rotateByX(-cog_rotation * static_cast<float>(delta_time));

			// Update physics step:
			bullet_dynamics_world->stepSimulation(static_cast<float>(delta_time));

			// Check for contact:
			bullet_dynamics_world->contactPairTest(barrel_rigid_body, wall_rigid_body, collision_callback);

			if (collision_callback.has_hit)
			{
				// Has hit the wall, score 1 point and reset barrel position:
				score++;
				is_launched = false;

				barrel_rigid_body->clearForces();
				barrel_rigid_body->setLinearVelocity(zero_vector);
				barrel_rigid_body->setAngularVelocity(zero_vector);
				barrel_rigid_body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2.2f, 1)));

				collision_last_time = static_cast<float>(glfwGetTime());
			}
			else
			{
				bullet_dynamics_world->contactPairTest(barrel_rigid_body, ground_rigid_body, collision_callback);

				if (collision_callback.has_hit)
				{
					// Has hit the ground, lose 1 barrel and reset barrel position:
					is_launched = false;

					remaining_lives--;
					if (remaining_lives <= 0)
					{
						current_game_state = GAME_OVER;

						camera.setPosition(0.f, 3.f, 4.f);
						camera.setRotation(-5.f, 0.f, 0.f);
					}

					barrel_rigid_body->clearForces();
					barrel_rigid_body->setLinearVelocity(zero_vector);
					barrel_rigid_body->setAngularVelocity(zero_vector);
					barrel_rigid_body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2.2f, 1)));

					collision_last_time = static_cast<float>(glfwGetTime());
				}
			}

			collision_callback.has_hit = false;

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

			if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				current_game_state = GAME_OVER;
				camera.setRotation(-5.f, 0.f, 0.f);
				camera.setPosition(0.f, 3.f, 4.f);
			}

			// Barrel launch:
			barrel_rigid_body->getMotionState()->getWorldTransform(launch_trans);
			corrected_force = (launch_trans * relative_force) - launch_trans.getOrigin();

			if (!is_launched &&
				(static_cast<float>(glfwGetTime()) - collision_last_time > launch_timeout) &&
				glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				barrel_rigid_body->activate();
				barrel_rigid_body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), barrel_rigid_body->getCenterOfMassPosition()));
				barrel_rigid_body->setLinearVelocity(btVector3(0.f, -50.f, -80.f));
				is_launched = true;
			}

			// Update the barrel transform:
			barrel_rigid_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(aligned_model_matrix));

			// Draw the barrel:
			sha.setActive(true);
			tex.setActive(true);
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

			// Update the pole transform:
			auto pole_pos = pole_rigid_body->getCenterOfMassPosition();
			
			if ((pole_move_left && pole_pos.getX() > 8) ||
				(!pole_move_left && pole_pos.getX() < -8))
			{
				pole_move_left = !pole_move_left;
			}

			pole_pos.setX(pole_pos.getX() + ((pole_move_left) ? 5 : -5) * static_cast<float>(delta_time));
			pole_rigid_body->setCenterOfMassTransform(btTransform(btQuaternion(0, 0, 0, 1), pole_pos));
			pole_rigid_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(aligned_model_matrix));

			// Draw pole obstacle:
			sha2.setActive(true);
			sha2.setUniformAttribute("model_matrix", aligned_model_matrix);
			pole_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, pole_mesh.getVertexCount());

			// Draw the animated cogs on ramp sides:
			sha2.setActive(true);
			sha2.setUniformAttribute("model_matrix", cog1.getCachedGlobalMatrix());
			cog_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, cog_mesh.getVertexCount());

			sha2.setActive(true);
			sha2.setUniformAttribute("model_matrix", cog2.getCachedGlobalMatrix());
			cog_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, cog_mesh.getVertexCount());

			sha2.setActive(true);
			sha2.setUniformAttribute("model_matrix", cog1.getCachedGlobalMatrix() * cog3.getCachedGlobalMatrix());
			cog_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, cog_mesh.getVertexCount());

			sha2.setActive(true);
			sha2.setUniformAttribute("model_matrix", cog2.getCachedGlobalMatrix() * cog4.getCachedGlobalMatrix());
			cog_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, cog_mesh.getVertexCount());

			sha2.setActive(true);
			sha2.setUniformAttribute("model_matrix", cog5.getCachedGlobalMatrix());
			cog_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, cog_mesh.getVertexCount());

			sha2.setActive(true);
			sha2.setUniformAttribute("model_matrix", cog6.getCachedGlobalMatrix());
			cog_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, cog_mesh.getVertexCount());

			// Update the wall transform:
			wall_rigid_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(aligned_model_matrix));

			// Draw the wall:
			sha.setActive(true);
			tex2.setActive(true);
			sha.setUniformAttribute("model_matrix", aligned_model_matrix); // barrel_transform.getCachedGlobalMatrix());
			wall_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, wall_mesh.getVertexCount());

			// Render text:
			str_stream << "Current Score: ";
			str_stream << score;

			update_text(score_text, str_stream.str().c_str());
			str_stream.clear();
			str_stream.str("");

			str_stream << "Remaining Barrels: ";
			str_stream << remaining_lives;

			update_text(barrel_text, str_stream.str().c_str());
			str_stream.clear();
			str_stream.str("");

			draw_text(score_text);
			draw_text(barrel_text);
		}
		else
		{
			barrel_icon_trans.rotateByX(-30.f * static_cast<float>(delta_time));

			if (glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_ENTER))
			{
				if (play_hover)
				{
					current_game_state = GAME_PLAY;
					score = 0;
					remaining_lives = 3;
					is_launched = false;

					barrel_rigid_body->clearForces();
					barrel_rigid_body->setLinearVelocity(zero_vector);
					barrel_rigid_body->setAngularVelocity(zero_vector);
					barrel_rigid_body->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2.2f, 1)));

					camera.setRotation(-5.f, 0.f, 0.f);
					camera.setPosition(0.f, 3.f, 4.f);
					update_text(play_text, "Play Again");
					update_text(over_text, "GAME OVER!");
				}
				else
				{
					return EXIT_SUCCESS;
				}
			}

			if ((static_cast<float>(glfwGetTime()) - input_last_time > input_timeout) &&
				(glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_UP) ||
				glfwGetKey(game_window->getRawWindowHandle(), GLFW_KEY_DOWN)))
			{
				play_hover = !play_hover;
				barrel_icon_trans.setPosition(((play_hover) ? play_pos : quit_pos));
				input_last_time = static_cast<float>(glfwGetTime());
			}

			// Render the barrel:
			sha.setActive(true);
			tex.setActive(true);
			sha.setUniformAttribute("model_matrix", barrel_icon_trans.getCachedGlobalMatrix());
			barrel_mesh.setActive(true, true);
			glDrawArrays(GL_TRIANGLES, 0, barrel_mesh.getVertexCount());

			// Render game text:
			draw_text(play_text);
			draw_text(quit_text);
			draw_text(over_text);
		}

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
