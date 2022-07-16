#pragma once

#include "core.h"

#include <glm/glm.hpp>

namespace Noor
{

	struct Shader
	{
		uint32_t id;
	};

	Ref<Shader> create_shader_from_files(const char* vs_file_path, const char* fs_file_path);
	void bind_shader(Ref<Shader> shader);

	void set_shader_uniform_mat4(Ref<Shader> shader, const char* name, glm::mat4& mat);
	void set_shader_uniform_int(Ref<Shader> shader, const char* name, int32_t value);

}    
