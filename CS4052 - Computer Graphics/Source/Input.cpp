/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Utility class for receiving GLFW input events.
Is a singleton class.
*/
#include "Input.hpp"
#include "OpenGL.hpp"
#include <iostream>


// Singleton functions:
Input* Input::m_singleton_instance = nullptr;

Input& Input::getInstance()
{
	return *((m_singleton_instance) ? m_singleton_instance : (m_singleton_instance = new Input()));
}



Input::Input()
{
	m_key_states.clear(); // ensure emptiness
}


Input::~Input()
{
	m_key_states.clear();
}


void Input::clearInputs()
{
	// Iterate over valid map entries and reset them to invalid states:
	for (auto& elem : m_key_states)
	{
		elem.second = UNDEFINED_KEY_STATE;
	}
}


void Input::injectKeyEvent(int p_key_code, int p_key_state)
{
	m_key_states[p_key_code] = p_key_state;
}


// TODO - more event processing


bool Input::isKeyPressed(int p_key_code, bool p_repeat)
{
	return ((p_repeat) ? this->getRawKeyState(p_key_code) == GLFW_REPEAT || this->getRawKeyState(p_key_code) == GLFW_PRESS :
						 this->getRawKeyState(p_key_code) == GLFW_PRESS);
}


bool Input::isKeyReleased(int p_key_code)
{
	bool result = this->getRawKeyState(p_key_code) == GLFW_RELEASE;

	// Reset input as soon as detected:
	m_key_states[p_key_code] = UNDEFINED_KEY_STATE;

	return result;
}


int Input::getRawKeyState(int p_key_code)
{
	auto key_iter = m_key_states.find(p_key_code);

	if (key_iter != m_key_states.end())
	{
		return key_iter->second;
	}
	else
	{
		return UNDEFINED_KEY_STATE;
	}
}


void keyEvent(GLFWwindow* p_window, int p_key, int p_code, int p_action, int p_mod)
{
	Input::getInstance().injectKeyEvent(p_key, p_action);
}
