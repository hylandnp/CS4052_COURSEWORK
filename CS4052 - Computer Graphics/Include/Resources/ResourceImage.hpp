/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)
*/
#pragma once
#include "Resources/ResourceBase.hpp"
#include <cstddef>
#include <string>


/*
Software image class.
This represents 32bit 'BGRA' image byte data stored in main memory.
*/
class ResourceImage : public ResourceBase
{
public:
	// Constructor:
	ResourceImage();

	// Load function(s):
	bool loadFromFile(const std::string& p_file_src);
	bool loadFromMemory(const std::string& p_str_src);

	// Inherited disposal function:
	void unLoad();

	// Getter function(s):
	const unsigned char* getRawImagePtr();
	std::size_t getWidthInPixels();
	std::size_t getHeightInPixels();
	std::size_t getSizeInBytes();
private:
	unsigned char* m_img_data;
	std::size_t m_width,
				m_height,
				m_img_size;
};
