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
		stream << "\tOpened log file...\n";
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
		stream << "\tClosed log file...\n";
		stream.flush();
	}
	
	stream.close();
}


void _Logger::write(const char* p_msg_str,
				    ...)
{
	char msg_buffer[LOG_MSG_BUFFER_SIZE];
	va_list arg_list;

	// Parse variadic arguments:
	va_start(arg_list, p_msg_str);
	vsprintf(msg_buffer, p_msg_str, arg_list);
	va_end(arg_list);

	// Write out to console (if debug build):
	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << msg_buffer;
		std::cout << std::endl;
	#endif // _DEBUG || !NDEBUG

	// Write out to log file:
	if (m_output_stream &&
		m_output_stream->is_open() &&
		m_output_stream->good() &&
		!m_output_stream->bad())
	{
		m_output_stream->write(msg_buffer, strlen(msg_buffer));
		m_output_stream->write("\n", 1);
	}
}


void _Logger::writeError(const std::string& p_file_name,
						 const std::string& p_func_name,
				    	 std::size_t p_line_no,
				    	 const char* p_err_str,
				    	 ...)
{
	char msg_buffer[LOG_MSG_BUFFER_SIZE],
		 prefix_buffer[LOG_MSG_BUFFER_SIZE];
	va_list arg_list;

	// Parse variadic arguments
	va_start(arg_list, p_err_str);
	vsprintf(msg_buffer, p_err_str, arg_list);
	va_end(arg_list);

	// Format error message prefix:
	sprintf(prefix_buffer,
			"\nERROR: [File: '%s', Function: '%s', Line: '%u']:\n",
			p_file_name.c_str(),
			p_func_name.c_str(),
			p_line_no);

	// Write out to console (if debug build):
	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << prefix_buffer;
		std::cout << msg_buffer;
		std::cout << "\n\n";
	#endif // _DEBUG || !NDEBUG

	// Write out to log file:
	if (m_output_stream &&
		m_output_stream->is_open() &&
		m_output_stream->good() &&
		!m_output_stream->bad())
	{
		m_output_stream->write(prefix_buffer, strlen(prefix_buffer));
		m_output_stream->write(msg_buffer, strlen(msg_buffer));
		m_output_stream->write("\n\n", 2);
	}
}



bool _Logger::isOpen()
{
	return (m_output_stream && m_output_stream->is_open());
}
