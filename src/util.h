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
	void free_loaded_file(File file);

}
	
}
