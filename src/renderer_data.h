#pragma once

#include "buffers.h"
#include "framebuffer.h"
#include "shader.h"

namespace Noor
{

	struct RendererData
	{
		Ref<Shader> pbr_shader;
		Ref<Shader> tonemapping_shader;
		Ref<Shader> equi_to_cube_shader;
		Ref<Shader> irrad_conv_shader;
		Ref<Shader> DFG_shader;
		Ref<Shader> rad_prefilter_shader;
		Ref<Shader> skybox_shader;

		Ref<Framebuffer> scratch_framebuffer;

		Ref<VertexArray> cube_vao;
		Ref<VertexArray> inv_cube_vao;
		Ref<VertexArray> unit_quad_vao;

		Ref<Texture2D> DFG_lut;

		glm::mat4 render_to_cube_mats[6];
	};

	extern Ref<RendererData> g_renderer_data;
}    
