#pragma once

#include "defs.h"

#include <string>

#include <glm/glm.hpp>

namespace Noor
{

	struct Shader
	{
		std::string vs_file_path;
		std::string fs_file_path;
		uint32_t id;
	};

	Ref<Shader> create_shader_from_files(const char* vs_file_path, const char* fs_file_path);
	bool recompile_shader(Ref<Shader> shader);
	void bind_shader(Ref<Shader> shader);

	void set_shader_uniform_vec3(Ref<Shader> shader, const char *name, glm::vec3 vec);
	void set_shader_uniform_mat4(Ref<Shader> shader, const char* name, glm::mat4& mat);
	void set_shader_uniform_int(Ref<Shader> shader, const char* name, int32_t value);
	void set_shader_uniform_float(Ref<Shader> shader, const char* name, float value);
	
	void delete_shader(Ref<Shader> shader);

}    
