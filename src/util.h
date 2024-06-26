#pragma once

#include "defs.h"
#include <cstdint>

namespace Noor
{

namespace Util
{

	struct File
	{
		uint32_t size;
		void* data;
	};

	bool file_exists(const char* file_path);
	void write_file(const char* file_path, File file);
	File load_file(const char* file_path);
	File load_file_null_terminated(const char* file_path);
	void free_loaded_file(File file);

	struct Image
	{
		uint32_t width, height;
		DataType type;
		uint32_t channels;
		void* data;
	};

	Image load_image(const char* file_path, uint32_t desired_channels = 0, bool flip_vertically = true);
	Image flip_image_vertically(Image image);
	void free_image(Image image);

}
	
}
