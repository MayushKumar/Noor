#include "util.h"

#include <fstream>

namespace Noor
{

namespace Util
{

	File load_file(const char* file_path)
	{
		File file;
		std::ifstream fs = std::ifstream(file_path, std::ios_base::binary);
		fs.seekg(0, std::ios::end);
		file.size = fs.tellg();

		file.data = malloc(file.size);
		fs.seekg(0, std::ios::beg);
		fs.read((char*)file.data, file.size);
		fs.close();
		return file;
	}

	void free_loaded_file(File file)
	{
		free(file.data);
	}
	
}
	
}	
