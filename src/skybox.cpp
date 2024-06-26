#include "skybox.h"
#include "graphics_api.h"
#include "shader.h"
#include "buffers.h"
#include "framebuffer.h"
#include "renderer_data.h"

namespace Noor {

	// Make sure to bind the shader and set uniforms yourself
	// Only the u_orient_mat uniform of the shader will be set by this function
	void render_to_cubemap(Ref<CubeMap> cubemap, uint32_t level, Ref<Shader> shader)
	{
		Noor::push_framebuffer(g_renderer_data->scratch_framebuffer);
		uint32_t viewport_dim = cubemap->props.dimension / std::pow(2, level);
		Noor::push_viewport(viewport_dim, viewport_dim);
		Noor::set_depth_mask(false);

		for(uint32_t i = 0; i < 6; i++)
		{
			Noor::attach_color_cubemap_face(g_renderer_data->scratch_framebuffer, cubemap, 0, i, level);
			Noor::set_shader_uniform_mat4(shader, "u_orient_mat", g_renderer_data->render_to_cube_mats[i]);
			Noor::clear_color_buffer(g_renderer_data->scratch_framebuffer, 0);
			Noor::draw_indexed(g_renderer_data->unit_quad_vao);
		}

		Noor::set_depth_mask(true);
		Noor::pop_viewport();
		Noor::pop_framebuffer();
	}
	
}
