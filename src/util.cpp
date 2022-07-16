#include "util.h"

#include <fstream>

#include <stb_image.h>

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

	Image load_image(const char* file_path, uint32_t desired_channels, bool flip_vertically)
	{
		stbi_set_flip_vertically_on_load(flip_vertically);

		Image image;
		int x, y, channels;
		image.data = stbi_load(file_path, &x, &y, &channels, desired_channels);
		image.width = x;
		image.height = y;
		image.channels = channels;

		return image;
	}

	void free_image(Image image)
	{
		stbi_image_free(image.data);
	}
	
}
	
}	
