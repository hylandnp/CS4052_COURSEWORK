/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Utility/Singleton.hpp"

#include <string>
#include <iosfwd>
#include <cstdarg>
#include <cstddef>

#define LOG_MSG_BUFFER_SIZE 1024


/*
File logging class.
Utility class used to output debugging messages to a file.
*/
class _Logger
{
public:
	// Constuctor/destructor:
	_Logger();
	~_Logger();

	// File access function(s):
	bool openFile(const std::string& p_to_open);
	void closeFile();

	// Logging function(s):
	void write(const char* p_msg_str,
			   ...);
	void writeError(const std::string& p_file_name,
					const std::string& p_func_name,
				    std::size_t p_line_no,
				    const char* p_err_str,
				    ...);

	// Getter(s):
	bool isOpen();
private:
	std::ofstream* m_output_stream;
};


// Singleton instance of class:
typedef Singleton<_Logger> Logger;
