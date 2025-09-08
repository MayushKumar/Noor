#include "util.h"

#include "defs.h"
#include "log.h"

#include <fstream>
#include <cstring>
#include <unistd.h>

#include <stb_image.h>
#include <stb_image_write.h>

namespace Noor
{

namespace Util
{

	bool file_exists(const char* file_path)
	{
		return access(file_path, 0)	== 0;
	}

	void write_file(const char* file_path, File file)
	{
		std::ofstream fs(file_path, std::ios::binary);
		fs.write((char*)file.data, file.size);
		fs.close();
	}

	File load_file(const char* file_path)
	{
		File file;
		std::ifstream fs = std::ifstream(file_path, std::ios_base::binary);

		if (!fs)
		{
			NOOR_CORE_ERROR("Could not load file: {}", file_path);
			return file;
		}

		fs.seekg(0, std::ios::end);
		file.size = fs.tellg();

		file.data = malloc(file.size);
		fs.seekg(0, std::ios::beg);
		fs.read((char*)file.data, file.size);
		fs.close();
		return file;
	}

	File load_file_null_terminated(const char* file_path)
	{
		File file;
		std::ifstream fs = std::ifstream(file_path, std::ios_base::binary);

		if (!fs)
		{
			NOOR_CORE_ERROR("Could not load file: {}", file_path);
			return file;
		}

		fs.seekg(0, std::ios::end);
		file.size = (uint32_t)fs.tellg();

		file.data = malloc(file.size + 1);
		fs.seekg(0, std::ios::beg);
		fs.read((char*)file.data, file.size);
		*((char*)file.data + file.size) = '\0'; // appending the null character
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

		if (stbi_is_hdr(file_path))
		{
			image.data = stbi_loadf(file_path, &x, &y, &channels, 0);
			image.type = DataType::F32;
		}
		else
		{
			image.data = stbi_load(file_path, &x, &y, &channels, desired_channels);
			image.type = DataType::U8;
		}

		if (!image.data)
			NOOR_CORE_ERROR("Could not load image: {0}", file_path);
		image.width = x;
		image.height = y;
		image.channels = channels;

		return image;
	}

	Image flip_image_vertically(Image image)
	{
		Image result = image;

		result.data = malloc(image.width * image.height * image.channels);

		uint32_t type_size = data_type_size(image.type);

		for (int i = 0; i < image.height; i++)
		{
			memcpy((char*)result.data + i * image.width * image.channels * type_size,
				   (char*)image.data + (image.height - i - 1) * image.width * image.channels * type_size,
					image.width * image.channels * type_size);
		}

		return result;
	}

	void free_image(Image image)
	{
		// stbi_image_free is just free
		free(image.data);
	}
	
}
	
}
