/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Utility/NonCopyable.hpp"


/*
Templated singleton base class.
Used to create singleton instances of manager classes.
*/
template <class C>
class Singleton : public NonCopyable
{
public:
	// Retrieve the singleton instance:
	static C& getInstance()
	{
		return *((m_singleton_instance) ? m_singleton_instance : (m_singleton_instance = new C()));
	}

	// Initialise the singleton instance if null:
	static bool forceCreateInstance()
	{
		if (!m_singleton_instance) m_singleton_instance = new C();
		return (m_singleton_instance != nullptr);
	}

	// Destroy the singleton instance if initialised:
	static bool disposeOfInstance()
	{
		if (m_singleton_instance) delete m_singleton_instance;
		return (m_singleton_instance == nullptr);
	}

	// Check if the instance has been created:
	static bool hasValidInstance()
	{
		return (m_singleton_instance != nullptr);
	}
private:
	Singleton() {};
	~Singleton() {};

	static C* m_singleton_instance;
};


// Instance is initially null:
template<class C>
C* Singleton<C>::m_singleton_instance = nullptr;
