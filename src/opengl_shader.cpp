#include "shader.h"

#include "defs.h"
#include "log.h"
#include "util.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Noor
{

	static std::unordered_map<uint32_t, std::unordered_map<const char*, uint32_t>> gs_uniform_location_cache;

	Ref<Shader> create_shader_from_files(const char* vs_file_path, const char* fs_file_path)
	{
		Util::File vs_src = Util::load_file_null_terminated(vs_file_path);
		Util::File fs_src = Util::load_file_null_terminated(fs_file_path);

		uint32_t vs_id = glCreateShader(GL_VERTEX_SHADER);
		uint32_t fs_id = glCreateShader(GL_FRAGMENT_SHADER);

		char* tmp_vs_src_ptr = (char*)vs_src.data;
		glShaderSource(vs_id, 1, &tmp_vs_src_ptr, 0);
		glCompileShader(vs_id);
		int vs_compile_status = 0;
		glGetShaderiv(vs_id, GL_COMPILE_STATUS, &vs_compile_status);

		char* tmp_fs_src_ptr = (char*)fs_src.data;
		glShaderSource(fs_id, 1, &tmp_fs_src_ptr, 0);
		glCompileShader(fs_id);
		int fs_compile_status = 0;
		glGetShaderiv(fs_id, GL_COMPILE_STATUS, &fs_compile_status);

		Util::free_loaded_file(vs_src);
		Util::free_loaded_file(fs_src);

		bool failed = false;
		if (vs_compile_status == GL_FALSE)
		{
			NOOR_CORE_ERROR("Could not compile vertex shader : {0}", vs_file_path);
			failed = true;
		}

		if (fs_compile_status == GL_FALSE)
		{
			NOOR_CORE_ERROR("Could not compile fragment shader : {0}", fs_file_path);
			failed = true;
		}

		if (failed)
		{
			glDeleteShader(vs_id);
			glDeleteShader(fs_id);
			return nullptr;
		}

		uint32_t program_id = glCreateProgram();
		glAttachShader(program_id, vs_id);
		glAttachShader(program_id, fs_id);

		glLinkProgram(program_id);
		int program_link_status = 0;
		glGetProgramiv(program_id, GL_LINK_STATUS, &program_link_status);

		if (program_link_status == GL_FALSE)
		{
			glDeleteShader(vs_id);
			glDeleteShader(fs_id);
			glDeleteProgram(program_id);
			NOOR_CORE_ERROR("Could not link shader program.");
			return nullptr;
		}

		glDetachShader(program_id, vs_id);
		glDetachShader(program_id, fs_id);
		glDeleteShader(vs_id);
		glDeleteShader(fs_id);

		gs_uniform_location_cache.emplace(program_id, std::unordered_map<const char*, uint32_t>());

		Ref<Shader> shader = CreateRef<Shader>();
		shader->id = program_id;
		shader->vs_file_path = vs_file_path;
		shader->fs_file_path = fs_file_path;

		return shader;
	}

	bool recompile_shader(Ref<Shader> shader)
	{
		Ref<Shader> temp = create_shader_from_files(shader->vs_file_path.c_str(), shader->fs_file_path.c_str());
		if(!temp) return false;
		glDeleteProgram(shader->id);
		shader->id = temp->id;
		return true;
	}

	void bind_shader(Ref<Shader> shader)
	{
		glUseProgram(shader->id);
	}

	uint32_t get_uniform_location(Ref<Shader> shader, const char* name)
	{
		std::unordered_map<const char*, uint32_t>& uniform_map = gs_uniform_location_cache[shader->id];
		auto location_it = uniform_map.find(name);
		uint32_t location = 0;
		if (location_it == uniform_map.end())
		{
			location = glGetUniformLocation(shader->id, name);
			uniform_map.emplace(name, location);
		} else
		{
			location = location_it->second;
		}

		return location;
	}

	// void set_shader_uniform(Ref<Shader> shader, const ShaderUniform& uniform)
	// {
	// 	switch(uniform.type)
	// 	{
	// 	case DataType::F32X3:
	// 		glProgramUniform3fv(shader->id, get_uniform_location(shader, uniform.name.c_str()), uniform.data);
	// 		glProgramUniform3fv(shader->id, get_uniform_location(shader, uniform.name.c_str()), uniform.data);
			
	// 	}
	// }

	void set_shader_uniform_vec3(Ref<Shader> shader, const char *name, glm::vec3 vec)
	{
		glProgramUniform3fv(shader->id, get_uniform_location(shader, name), 1, glm::value_ptr(vec));
	}

	void set_shader_uniform_mat4(Ref<Shader> shader, const char* name, glm::mat4& mat)
	{
		glProgramUniformMatrix4fv(shader->id, get_uniform_location(shader, name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void set_shader_uniform_int(Ref<Shader> shader, const char* name, int32_t value)
	{
		glProgramUniform1i(shader->id, get_uniform_location(shader, name), value);
	}

	void set_shader_uniform_float(Ref<Shader> shader, const char* name, float value)
	{
		glProgramUniform1f(shader->id, get_uniform_location(shader, name), value);
	}

	void delete_shader(Ref<Shader> shader)
	{
		glDeleteProgram(shader->id);
	}

}
