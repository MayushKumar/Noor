#pragma once

#include "core.h"

namespace Noor
{

	struct Shader
	{
		uint32_t id;
	};

	Ref<Shader> create_shader_from_files(const char* vs_file_path, const char* fs_file_path);
	void bind_shader(Ref<Shader> shader);
	
}    
