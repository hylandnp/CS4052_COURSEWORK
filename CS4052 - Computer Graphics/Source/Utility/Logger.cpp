/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#include "Utility/Logger.hpp"
#include <fstream>

#if defined(_DEBUG) || !defined(NDEBUG)
	#include <iostream>
#endif // _DEBUG || !NDEBUG


_Logger::_Logger()
{
	// Default initialisation...
	m_output_stream = new std::ofstream();
}


_Logger::~_Logger()
{
	// Garbage collection...
	this->closeFile();
	delete m_output_stream;
}


bool _Logger::openFile(const std::string& p_to_open)
{
	this->closeFile(); // close previous file
	auto& stream = *m_output_stream;

	stream.open(p_to_open, std::ios::out | std::ios::trunc);
	
	if (stream.is_open() && stream.good() && !stream.bad())
	{
		stream << "Opened log file...\n";
		return true; // successful
	}
	else
	{
		stream.close();
		return false;
	}
}


void _Logger::closeFile()
{
	// Print last log file message and close stream:
	auto& stream = *m_output_stream;

	if (stream.is_open())
	{
		stream << "Closed log file...\n";
		stream.flush();
	}
	
	stream.close();
}
