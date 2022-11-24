#pragma once

#include <string>

namespace Noor
{

namespace Util
{

	struct File
	{
		uint32_t size;
		void* data;
	};

	File load_file(const char* file_path);
	File load_file_null_terminated(const char* file_path);
	void free_loaded_file(File file);

	struct Image
	{
		uint32_t width, height;
		uint32_t channels;
		void* data;
	};

	Image load_image(const char* file_path, uint32_t desired_channels = 0, bool flip_vertically = true);
	void free_image(Image image);

}
	
}
