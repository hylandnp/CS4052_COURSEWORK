/*
CS4052 - Computer Graphics
NEIL HYLAND (11511677)

Based on FreeImage tutorial/sample loading code.
*/
#include "Resources/ResourceImage.hpp"
#include <cstdlib>
#include <FreeImage.h>


ResourceImage::ResourceImage() :
	m_img_data(nullptr),
	m_width(0),
	m_height(0),
	m_img_size(0)
{
	// Initialisation...
	ResourceBase::ResourceBase(ResourceType::IMAGE);
}


bool ResourceImage::loadFromFile(const std::string& p_file_src)
{
	// Attempt to deduce image file format:
	auto img_format = FreeImage_GetFileType(p_file_src.c_str(), 0);
	if (img_format == FIF_UNKNOWN) img_format = FreeImage_GetFIFFromFilename(p_file_src.c_str());

	// Check if we can read the given file:
	if (img_format == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(img_format))
	{
		// TODO - err message
		return false;
	}

	// Attempt to load the file:
	auto fif_bitmap = FreeImage_Load(img_format, p_file_src.c_str());

	if (!fif_bitmap)
	{
		// TODO - err message
		return false;
	}

	// Convert file to 32bits if not already:
	if(FreeImage_GetBPP(fif_bitmap) != 32)
	{
		auto temp_bitmap = fif_bitmap;
		fif_bitmap = FreeImage_ConvertTo32Bits(fif_bitmap);
		FreeImage_Unload(temp_bitmap);
	}

	// Get image array info:
	m_width = FreeImage_GetWidth(fif_bitmap);
	m_height = FreeImage_GetHeight(fif_bitmap);
	m_img_size = 4 * m_width * m_height;

	// Store image bytes in array and return from function:
	m_img_data = new unsigned char[m_img_size];
	std::memcpy(m_img_data, FreeImage_GetBits(fif_bitmap), m_img_size);

	FreeImage_Unload(fif_bitmap);
	return (m_is_loaded = true); // successful
}


bool ResourceImage::loadFromMemory(const std::string& p_str_src)
{
	// TODO

	return (m_is_loaded = true); // successful
}


void ResourceImage::unLoad()
{
	if (m_img_data) delete[] m_img_data;

	m_width = 0;
	m_height = 0;
	m_img_size = 0;

	ResourceBase::unLoad();
}


const unsigned char* ResourceImage::getRawImagePtr()
{
	return m_img_data;
}


std::size_t ResourceImage::getWidthInPixels()
{
	return m_width;
}


std::size_t ResourceImage::getHeightInPixels()
{
	return m_height;
}


std::size_t ResourceImage::getSizeInBytes()
{
	return m_img_size;
}
