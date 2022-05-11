#include "shader.h"

#include "core.h"
#include "util.h"

#include <fstream>

#include <glad/glad.h>

namespace Noor
{

	Ref<Shader> create_shader_from_files(const char* vs_file_path, const char* fs_file_path)
	{
		Util::File vs_src = Util::load_file(vs_file_path);
		Util::File fs_src = Util::load_file(fs_file_path);

		uint32_t vs_id = glCreateShader(GL_VERTEX_SHADER);
		uint32_t fs_id = glCreateShader(GL_FRAGMENT_SHADER);

		char* tmp_vs_src_ptr = (char*)vs_src.data;
		glShaderSource(vs_id, 1, &tmp_vs_src_ptr, 0);
		Util::free_loaded_file(vs_src);
		glCompileShader(vs_id);
		int vs_compile_status = 0;
		glGetShaderiv(vs_id, GL_COMPILE_STATUS, &vs_compile_status);

		char* tmp_fs_src_ptr = (char*)fs_src.data;
		glShaderSource(fs_id, 1, &tmp_fs_src_ptr, 0);
		Util::free_loaded_file(fs_src);
		glCompileShader(fs_id);
		int fs_compile_status = 0;
		glGetShaderiv(fs_id, GL_COMPILE_STATUS, &fs_compile_status);

		if (vs_compile_status == GL_FALSE)
		{
			glDeleteShader(vs_id);
			glDeleteShader(fs_id);
			NOOR_CORE_ERROR("Could not compile vertex shader : {0}", vs_file_path);
			return nullptr;
		}

		if (vs_compile_status == GL_FALSE)
		{
			glDeleteShader(vs_id);
			glDeleteShader(fs_id);
			NOOR_CORE_ERROR("Could not compile fragment shader : {0}", fs_file_path);
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

		Ref<Shader> shader = CreateRef<Shader>();
		shader->id = program_id;

		return shader;
	}

	void bind_shader(Ref<Shader> shader)
	{
		glUseProgram(shader->id);
	}

}
