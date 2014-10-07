/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

General file and console logger utility class.
*/
#include "Log.hpp"

#include <fstream>
#include <iostream>
#include <cstring>


// Singleton functions:
Log* Log::m_singleton_instance = nullptr;

Log& Log::getInstance()
{
	return *((m_singleton_instance) ? m_singleton_instance : (m_singleton_instance = new Log()));
}


// Class functions:
Log::Log() :
	m_log_closed(false)
{
	m_file_stream = new std::ofstream();
}


Log::~Log()
{
	if (!m_log_closed) this->closeFile();
	if (m_file_stream) delete m_file_stream;
}


bool Log::openFile(const char* p_to_open)
{
	// Close previously-opened log file:
	this->closeFile();
	m_log_closed = false;

	char open_msg[] = "Opened log file stream...\n";

	// Open new log file:
	m_file_stream->open(p_to_open);

	if (m_file_stream->is_open() && m_file_stream->good() && !m_file_stream->bad())
	{
		// Write initial message to file:
		m_file_stream->write(open_msg, strlen(open_msg));
		m_file_stream->flush();
		return true;
	}
	else
	{
		#if defined(_DEBUG) || !defined(NDEBUG)
				std::cerr << "\nERROR: Failed to open new log file stream: '" << p_to_open << "', possible invalid path!\n\n";
		#endif // _DEBUG || !NDEBUG

		return false;
	}
}


void Log::closeFile()
{
	char close_msg[] = "Closing log file stream...\n";

	if (m_file_stream && m_file_stream->is_open())
	{
		// Write final message to file and close output stream:
		m_file_stream->write(close_msg, strlen(close_msg));
		m_file_stream->flush();
		m_file_stream->close();
		m_file_stream->clear();
	}

	m_log_closed = true;
}


void Log::writeMessage(const char* p_msg_str, ...)
{
	char msg_buffer[LOG_MSG_BUFFER_SIZE];
	va_list arg_list;

	// Parse variadic arguments:
	va_start(arg_list, p_msg_str);
	vsprintf(msg_buffer, p_msg_str, arg_list);
	va_end(arg_list);

	// Write out to file and console:
	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cout << msg_buffer;
	#endif // _DEBUG || !NDEBUG

	if (m_file_stream &&
		m_file_stream->is_open() &&
		m_file_stream->good() &&
		!m_file_stream->bad())
	{
		m_file_stream->write(msg_buffer, strlen(msg_buffer));
	}
}


void Log::writeError(const char* p_file_name,
					 std::size_t p_line_no,
					 const char* p_err_msg,
					 ...)
{
	char msg_buffer[LOG_MSG_BUFFER_SIZE],
		 err_buffer[LOG_MSG_BUFFER_SIZE];
	va_list arg_list;

	// Parse variadic arguments:
	va_start(arg_list, p_err_msg);
	vsprintf(msg_buffer, p_err_msg, arg_list);
	va_end(arg_list);

	// Format error message:
	sprintf(err_buffer, "\nERROR [File: '%s', Line: %u]:\n", p_file_name, p_line_no);

	// Write out to file and console:
	#if defined(_DEBUG) || !defined(NDEBUG)
		std::cerr << err_buffer;
		std::cerr << msg_buffer;
	#endif // _DEBUG || !NDEBUG

	if (m_file_stream &&
		m_file_stream->is_open() &&
		m_file_stream->good() &&
		!m_file_stream->bad())
	{
		m_file_stream->write(err_buffer, strlen(err_buffer));
		m_file_stream->write(msg_buffer, strlen(msg_buffer));
	}
}





