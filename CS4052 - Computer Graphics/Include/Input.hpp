/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Utility class for receiving GLFW input events.
Is a singleton class.
*/
#pragma once
#include <map>

struct GLFWwindow; // forward declaration

#define UNDEFINED_KEY_STATE -1


class Input
{
	public:
		static Input& getInstance();

		// Reset mapped input states:
		void clearInputs();

		// Event processing functions:
		void injectKeyEvent(int p_key_code, int p_key_state);

		// Event querying functions:
		bool isKeyPressed(int p_key_code, bool p_consume = false);
		bool isKeyReleased(int p_key_code);

		// Raw state access:
		int getRawKeyState(int p_key_code);
	private:
		// Internal constructor:
		Input();

		// Internal destructor:
		~Input();

		// Keyboard button states:
		std::map<int, int> m_key_states;

		// TODO - mouse input (if needed)

		// Singleton instance:
		static Input* m_singleton_instance;
};


// Global GLFW callback functions:
void keyEvent(GLFWwindow* p_window, int p_key, int p_code, int p_action, int p_mod);
