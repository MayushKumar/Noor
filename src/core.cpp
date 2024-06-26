#include "core.h"

#include "buffers.h"
#include "defs.h"
#include "framebuffer.h"
#include "shader.h"
#include "texture.h"
#include "util.h"
#include "window.h"
#include "graphics_api.h"

#include "renderer_data.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Noor
{

	void init_renderer_data();
	void init_shaders();
	void init_unit_quad_vao();
	void init_cube_vao();
	void init_mats();
	void init_framebuffers();
	void init_DFG_lut();

	void init(Window::WindowHandle window_handle, Window::WindowProps props)
	{
		GraphicsAPI::init(window_handle);

		push_viewport(props.width, props.height);
		init_renderer_data();
	}

	void init_renderer_data()
	{
		g_renderer_data = CreateRef<RendererData>();
		init_shaders();
		init_unit_quad_vao();
		init_cube_vao();
		init_mats();
		init_framebuffers();
		init_DFG_lut();
	}

	void init_shaders()
	{
		g_renderer_data->pbr_shader = Noor::create_shader_from_files("assets/shaders/pbr_vs.glsl",
																	 "assets/shaders/pbr_fs.glsl");
		g_renderer_data->tonemapping_shader = Noor::create_shader_from_files("assets/shaders/tonemapping_vs.glsl",
																			 "assets/shaders/tonemapping_fs.glsl");
		g_renderer_data->equi_to_cube_shader = Noor::create_shader_from_files("assets/shaders/equi_to_cube_vs.glsl",
																	 "assets/shaders/equi_to_cube_fs.glsl"); 
		g_renderer_data->irrad_conv_shader = Noor::create_shader_from_files("assets/shaders/irrad_conv_vs.glsl",
																	 "assets/shaders/irrad_conv_fs.glsl"); 
		g_renderer_data->DFG_shader = Noor::create_shader_from_files("assets/shaders/dfg_vs.glsl",
																	 "assets/shaders/dfg_fs.glsl"); 
		g_renderer_data->rad_prefilter_shader = Noor::create_shader_from_files("assets/shaders/rad_prefilter_vs.glsl",
																	 "assets/shaders/rad_prefilter_fs.glsl"); 
		g_renderer_data->skybox_shader = Noor::create_shader_from_files("assets/shaders/skybox_vs.glsl",
																	 "assets/shaders/skybox_fs.glsl"); 
	}

	void init_unit_quad_vao()
	{
		float vertices [4 * (3 + 2)] = {
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
			0.5f,  0.5f, 0.0f,  1.0f, 1.0f
		};

		uint32_t indices[6] = {0, 1, 2, 1, 3, 2};
		
		Ref<VertexBuffer> vertex_buffer = Noor::create_vertex_buffer((void*)vertices, sizeof(vertices),
												   {{"POSITION", NOOR_FLOAT, 3, 0},
													{"TEXCOORDS", NOOR_FLOAT, 2, 3 * sizeof(float)}});
		Ref<IndexBuffer> index_buffer = Noor::create_index_buffer((void*)indices, sizeof(indices), NOOR_UINT);
		g_renderer_data->unit_quad_vao = Noor::create_vertex_array();
		Noor::attach_vertex_buffer(g_renderer_data->unit_quad_vao, vertex_buffer, 0);
		Noor::attach_index_buffer(g_renderer_data->unit_quad_vao, index_buffer);
	}

	void init_cube_vao()
	{
		float vertices[] = {
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
		};

		uint16_t indices[] = {
			4,2,0,
			2,7,3,
			6,5,7,
			1,7,5,
			0,3,1,
			4,1,5,
			4,6,2,
			2,6,7,
			6,4,5,
			1,3,7,
			0,2,3,
			4,0,1
		};

		uint16_t inv_indices[] = {
			4,0,2,
			2,3,7,
			6,7,5,
			1,5,7,
			0,1,3,
			4,5,1,
			4,2,6,
			2,7,6,
			6,5,4,
			1,7,3,
			0,3,2,
			4,1,0
		};

		Ref<VertexBuffer> vertex_buffer = create_vertex_buffer(vertices, sizeof(vertices),
															   {{"POSITION", NOOR_FLOAT, 3, 0}});
		Ref<IndexBuffer> index_buffer = create_index_buffer(indices, sizeof(indices), NOOR_USHORT);
		Ref<IndexBuffer> inv_index_buffer = create_index_buffer(inv_indices, sizeof(inv_indices), NOOR_USHORT);

		g_renderer_data->cube_vao = create_vertex_array();
		attach_vertex_buffer(g_renderer_data->cube_vao, vertex_buffer, 0);
		attach_index_buffer(g_renderer_data->cube_vao, index_buffer);

		g_renderer_data->inv_cube_vao = create_vertex_array();
		attach_vertex_buffer(g_renderer_data->inv_cube_vao, vertex_buffer, 0);
		attach_index_buffer(g_renderer_data->inv_cube_vao, inv_index_buffer);
	}

	void init_mats()
	{
		glm::mat4 scale_mat = glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f, 2.0f, 1.0f));
		glm::mat4 trans_mat = glm::translate(scale_mat, glm::vec3(0.0f, 0.0f, -1.0f));

		glm::vec4 x_axis = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec4 y_axis = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
		glm::vec4 z_axis = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		glm::vec4 w_axis = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glm::mat4 invert_x = glm::mat4(-x_axis, y_axis, z_axis, w_axis);
		glm::mat4 invert_y = glm::mat4(x_axis, -y_axis, z_axis, w_axis);
		glm::mat4 invert_z = glm::mat4(x_axis, y_axis, -z_axis, w_axis);

		g_renderer_data->render_to_cube_mats[0] = invert_y * invert_z * glm::mat4_cast(glm::quat(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f))) * trans_mat;
		g_renderer_data->render_to_cube_mats[1] = invert_y * invert_z * glm::mat4_cast(glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f))) * trans_mat;
		g_renderer_data->render_to_cube_mats[2] = glm::mat4_cast(glm::quat(glm::vec3(glm::radians(90.0f), 0.0f ,0.0f))) * trans_mat;
		g_renderer_data->render_to_cube_mats[3] = glm::mat4_cast(glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f ,0.0f))) * trans_mat;
		g_renderer_data->render_to_cube_mats[4] = invert_y * invert_x * glm::mat4_cast(glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f))) * trans_mat;
		g_renderer_data->render_to_cube_mats[5] = invert_y * invert_x * glm::mat4_cast(glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f ))) * trans_mat;
	}

	void init_framebuffers()
	{
		g_renderer_data->scratch_framebuffer = Noor::create_framebuffer();
	}

	void init_DFG_lut()
	{
		if(Util::file_exists("assets/textures/dfg_lut.bin"))
		{
			Util::File file = Util::load_file("assets/textures/dfg_lut.bin");
			Util::Image image = {256, 256, NOOR_HALF_FLOAT, 2, file.data};
			g_renderer_data->DFG_lut = create_texture({256, 256, NOOR_RG16F, 0, NOOR_LINEAR, NOOR_LINEAR, NOOR_CLAMP_TO_EDGE}, image);
		} else
		{
			g_renderer_data->DFG_lut = create_texture({256, 256, NOOR_RG16F, 0, NOOR_LINEAR, NOOR_LINEAR, NOOR_REPEAT});
			attach_color_texture2D(g_renderer_data->scratch_framebuffer, g_renderer_data->DFG_lut, 0, 0);

			bind_shader(g_renderer_data->DFG_shader);

			push_framebuffer(g_renderer_data->scratch_framebuffer);
			push_viewport(256, 256);
			draw_indexed(g_renderer_data->unit_quad_vao);
			pop_viewport();
			pop_framebuffer();

			Util::Image image = get_texture_image(g_renderer_data->DFG_lut, 0);
			Util::write_file("assets/textures/dfg_lut.bin", {image.channels * image.width * image.height * data_type_size(image.type), image.data});
			Util::free_image(image);
		}
	}

}
