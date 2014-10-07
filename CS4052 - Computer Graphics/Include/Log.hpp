/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

General file and console logger utility class.
Is a singleton class.
*/
#pragma once

#include <cstdarg>
#include <iosfwd>
#include <cstddef>

#define LOG_MSG_BUFFER_SIZE 1024


class Log
{
	public:
		static Log& getInstance();

		// File initialisation:
		bool openFile(const char* p_to_open);

		// File de-initialisation:
		void closeFile();

		// Message logging:
		void writeMessage(const char* p_msg_str, ...);

		// Error message logging:
		void writeError(const char* p_file_name,
						std::size_t p_line_no,
						const char* p_err_msg,
						...);
	private:
		// Internal constructor:
		Log();

		// Internal destructor:
		~Log();

		// Member data:
		std::ofstream* m_file_stream;
		bool m_log_closed;

		// Singleton instance:
		static Log* m_singleton_instance;
};
