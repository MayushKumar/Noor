#include "Noor.h"

#include "camera_controller.h"
#include "defs.h"
#include "renderer_data.h"
#include "shader.h"
#include "texture.h"

#include <glm/gtc/matrix_transform.hpp>

#define RENDER_MODEL 1 

Noor::Window::WindowProps window_props;
Noor::Window::WindowHandle window_handle;

void game_loop();
void shutdown();
void key_callback(int32_t key, int32_t action, int32_t mods);
void mouse_button_callback(int32_t button, int32_t action, int32_t mods);
void mouse_position_callback(int32_t x, int32_t y);

Noor::Ref<Noor::Texture2D> texture;
Noor::Ref<Noor::Framebuffer> framebuffer;
Noor::Ref<Noor::Texture2D> framebuffer_color_tex;
Noor::Ref<Noor::Texture2D> framebuffer_depth_tex;
Noor::Ref<Noor::Texture2D> env;
Noor::Ref<Noor::Framebuffer> cubemap_framebuffer;
Noor::Ref<Noor::CubeMap> cubemap;
Noor::Ref<Noor::CubeMap> irrad_conv_cubemap;
Noor::Ref<Noor::CubeMap> rad_prefilter_cubemap;
Noor::Ref<Noor::Model> models[2];
glm::mat4 cubemap_proj_mat;
glm::mat4 model_mats[2];
glm::mat4 normal_mats[2];
uint32_t model_index = 0;

static const uint32_t c_WINDOW_WIDTH = 1920, c_WINDOW_HEIGHT = 1080;
static const uint32_t c_CUBEMAP_DIM = 1024;
static const uint32_t c_MODEL_COUNT = 2;

CameraController camera_controller;
int main()
{
	Noor::Log::init();
	window_props.width = c_WINDOW_WIDTH;
	window_props.height = c_WINDOW_HEIGHT;
	window_props.key_callback_fn = key_callback;
	window_props.mouse_button_callback_fn = mouse_button_callback;
	window_props.mouse_position_callback_fn = mouse_position_callback;
	window_handle = Noor::Window::create_window(window_props);
	Noor::init(window_handle, window_props);

	camera_controller = CameraController(window_handle, glm::radians(60.0f),
										 (float)c_WINDOW_WIDTH / (float)c_WINDOW_HEIGHT, 0.1f, 100.0f);

	// pbr_shader = Noor::create_shader_from_files("assets/shaders/pbr.vert", "assets/shaders/pbr.frag");

	Noor::Util::Image image = Noor::Util::load_image("assets/textures/texture.png");
	Noor::Texture2DProps texture_props;
	texture_props.internal_format = NOOR_RGBA8;
	texture_props.mip_count = 0;
	texture_props.min_filter = NOOR_LINEAR;
	texture_props.mag_filter = NOOR_NEAREST;
	texture_props.wrapping = NOOR_REPEAT;
	texture = Noor::create_texture(texture_props, image);
	Noor::Util::free_image(image);

#if RENDER_MODEL
	model_mats[1] = glm::rotate(glm::identity<glm::mat4>(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model_mats[0] = glm::identity<glm::mat4>();
	normal_mats[1] = glm::inverse(glm::transpose(model_mats[1]));
	normal_mats[0] = glm::inverse(glm::transpose(model_mats[0]));
	models[1] = Noor::load_model("assets/models/cerberus/gltf/Cerberus.gltf");
	models[0] = Noor::load_model("assets/models/chest/treasure_chest_2k.gltf");
#endif

	framebuffer = Noor::create_framebuffer();
	Noor::Texture2DProps props;
	props.width = 1920;
	props.height = 1080;
	props.internal_format = NOOR_RGB16F;
	props.min_filter = NOOR_NEAREST;
	props.mag_filter = NOOR_NEAREST;
	props.mip_count = 0;
	props.wrapping = NOOR_REPEAT;
	framebuffer_color_tex = Noor::create_texture(props);
	props.internal_format = NOOR_DEPTH_COMPONENT16;
	framebuffer_depth_tex = Noor::create_texture(props);
	Noor::attach_color_texture2D(framebuffer, framebuffer_color_tex, 0, 0);
	Noor::attach_depth_texture(framebuffer, framebuffer_depth_tex);

	Noor::set_clear_color({0.0f, 0.0f, 0.0f, 0.0f});


	// ---------------------------------------- Sky Box -----------------------------------------------------------------------
	Noor::Util::Image env_img = Noor::Util::load_image("assets/textures/environment/meadow_2_4k.hdr");
	// Noor::Util::Image env_img = Noor::Util::load_image("assets/textures/environment/chinese_garden_4k.hdr");
	// Noor::Util::Image env_img = Noor::Util::load_image("assets/textures/environment/hotel_rooftop_balcony_4k.hdr");
	// Noor::Util::Image env_img = Noor::Util::load_image("assets/textures/environment/oberer_kuhberg_4k.hdr");

	env = Noor::create_texture({0, 0, NOOR_RGB32F, 0, NOOR_LINEAR, NOOR_LINEAR, NOOR_CLAMP_TO_EDGE}, env_img);
	cubemap = Noor::create_cubemap({c_CUBEMAP_DIM, NOOR_RGB16F, 3, NOOR_LINEAR_MIPMAP_LINEAR, NOOR_LINEAR, NOOR_CLAMP_TO_EDGE});
	irrad_conv_cubemap = Noor::create_cubemap({c_CUBEMAP_DIM / 2, NOOR_RGB16F, 0, NOOR_LINEAR, NOOR_LINEAR, NOOR_CLAMP_TO_EDGE});
	rad_prefilter_cubemap = Noor::create_cubemap({128, NOOR_RGB16F, 4, NOOR_LINEAR_MIPMAP_LINEAR, NOOR_LINEAR, NOOR_CLAMP_TO_EDGE});
	Noor::gen_mipmaps(rad_prefilter_cubemap);

	// cubemap_framebuffer = Noor::create_framebuffer();

	Noor::bind_texture(env, 0);
	Noor::bind_shader(Noor::g_renderer_data->equi_to_cube_shader);
	Noor::set_shader_uniform_int(Noor::g_renderer_data->equi_to_cube_shader, "u_envmap", 0);
	Noor::render_to_cubemap(cubemap, 0, Noor::g_renderer_data->equi_to_cube_shader);
	Noor::gen_mipmaps(cubemap);

	Noor::bind_cubemap(cubemap, 0);
	Noor::bind_shader(Noor::g_renderer_data->irrad_conv_shader);
	Noor::set_shader_uniform_int(Noor::g_renderer_data->irrad_conv_shader, "u_envmap", 0);
	Noor::render_to_cubemap(irrad_conv_cubemap, 0, Noor::g_renderer_data->irrad_conv_shader);

	Noor::bind_cubemap(cubemap, 0);
	Noor::bind_shader(Noor::g_renderer_data->rad_prefilter_shader);
	Noor::set_shader_uniform_int(Noor::g_renderer_data->rad_prefilter_shader, "u_envmap", 0);
	for(uint32_t i = 0; i < 5; i++)
	{
		Noor::set_shader_uniform_float(Noor::g_renderer_data->rad_prefilter_shader, "u_roughness", i / 4.0f);
		Noor::render_to_cubemap(rad_prefilter_cubemap, i, Noor::g_renderer_data->rad_prefilter_shader);
	}

	// -----------------------------------------------------------------------------------------------------------------------

	game_loop();

	shutdown();
}

void game_loop() {
	while (!Noor::Window::window_should_close(window_handle))
	{
		Noor::Window::poll_events();

		camera_controller.update();

		Noor::Window::swap_buffers(window_handle);
		Noor::clear_color_buffer();
		Noor::clear_depth_buffer();
		
		Noor::push_framebuffer(framebuffer);
		Noor::clear_color_buffer(framebuffer, 0);
		Noor::clear_depth_buffer(framebuffer);

		// ---------------------------- SKYBOX RENDERING --------------------------------
		Noor::set_depth_mask(false);

		glm::mat4 skybox_mat = camera_controller.get_proj_matrix() * glm::inverse(glm::mat4_cast((camera_controller.get_orientation())));
		Noor::bind_shader(Noor::g_renderer_data->skybox_shader);
		Noor::set_shader_uniform_int(Noor::g_renderer_data->skybox_shader, "u_env_map", 0);

		Noor::set_shader_uniform_mat4(Noor::g_renderer_data->skybox_shader, "u_view_proj_mat", skybox_mat);
		Noor::bind_cubemap(cubemap, 0);
		Noor::draw_indexed(Noor::g_renderer_data->inv_cube_vao);

		Noor::set_depth_mask(true);

		// --------------------------- MODEL RENDERING ----------------------------------
#if RENDER_MODEL
		Noor::bind_shader(Noor::g_renderer_data->pbr_shader);
		Noor::set_shader_uniform_mat4(Noor::g_renderer_data->pbr_shader, "u_view_proj_mat", camera_controller.get_view_proj_matrix());
		Noor::set_shader_uniform_mat4(Noor::g_renderer_data->pbr_shader, "u_normal_mat", normal_mats[model_index]);
		Noor::set_shader_uniform_vec3(Noor::g_renderer_data->pbr_shader, "u_view_pos", camera_controller.get_position());
		Noor::set_shader_uniform_int(Noor::g_renderer_data->pbr_shader, "u_envmap", 3);
		Noor::set_shader_uniform_int(Noor::g_renderer_data->pbr_shader, "u_irradmap", 4);
		Noor::set_shader_uniform_int(Noor::g_renderer_data->pbr_shader, "u_radmap", 5);
		Noor::set_shader_uniform_int(Noor::g_renderer_data->pbr_shader, "u_dfglut", 6);
		Noor::bind_cubemap(cubemap, 3);
		Noor::bind_cubemap(irrad_conv_cubemap, 4);
		Noor::bind_cubemap(rad_prefilter_cubemap, 5);
		Noor::bind_texture(Noor::g_renderer_data->DFG_lut, 6);
		for (auto mesh : models[model_index]->meshes) {
			glm::mat4 model_mat = model_mats[model_index] * glm::translate(glm::identity<glm::mat4>(), mesh->translation);
			Noor::set_shader_uniform_mat4(Noor::g_renderer_data->pbr_shader, "u_model_mat", model_mat);
			Noor::set_shader_uniform_int(Noor::g_renderer_data->pbr_shader, "material.base_tex", 0);
			Noor::set_shader_uniform_int(Noor::g_renderer_data->pbr_shader, "material.normal_tex", 1);
			Noor::set_shader_uniform_int(Noor::g_renderer_data->pbr_shader, "material.metallic_roughness_tex", 2);
			Noor::set_shader_uniform_vec3(Noor::g_renderer_data->pbr_shader, "light.pos", {0.0f, 50.0f, 0.0f});
			Noor::bind_texture(mesh->material->base_tex, 0);
			Noor::bind_texture(mesh->material->normal_tex, 1);
			Noor::bind_texture(mesh->material->metallic_roughness_tex, 2);
			Noor::draw_indexed(mesh->vertex_array);
		}
#endif
		// --------------------------------------------------------------------------------

		Noor::pop_framebuffer();

		// -------------------------- FRAMEBUFFER RENDERING ------------------------------
		Noor::bind_shader(Noor::g_renderer_data->tonemapping_shader);
		Noor::set_shader_uniform_int(Noor::g_renderer_data->tonemapping_shader, "u_texture", 0);
		Noor::bind_texture(framebuffer_color_tex, 0);
		Noor::draw_indexed(Noor::g_renderer_data->unit_quad_vao);
	}
}

void key_callback(int32_t key, int32_t action, int32_t mods) {
	camera_controller.key_event(key, action, mods);

	if (key == NOOR_KEY_X && action == NOOR_PRESSED)
	{
		camera_controller.set_enabled(!camera_controller.is_enabled());
	}

	else if (key == NOOR_KEY_R && action == NOOR_PRESSED)
	{
		Noor::recompile_shader(Noor::g_renderer_data->pbr_shader);
	}
	else if (key == NOOR_KEY_S && action == NOOR_PRESSED)
	{
		Noor::recompile_shader(Noor::g_renderer_data->skybox_shader);
	}
	else if (key == NOOR_KEY_M && action == NOOR_PRESSED)
	{
		model_index = (model_index + 1) % c_MODEL_COUNT;
	}
	else if (key == NOOR_KEY_G && action == NOOR_PRESSED)
	{
		if(Noor::recompile_shader(Noor::g_renderer_data->rad_prefilter_shader))
		{
			Noor::bind_cubemap(cubemap, 0);
			Noor::bind_shader(Noor::g_renderer_data->rad_prefilter_shader);
			Noor::set_shader_uniform_int(Noor::g_renderer_data->rad_prefilter_shader, "u_envmap", 0);
			for(uint32_t i = 0; i < 5; i++)
			{
				Noor::set_shader_uniform_float(Noor::g_renderer_data->rad_prefilter_shader, "u_roughness", i / 4.0f);
				Noor::render_to_cubemap(rad_prefilter_cubemap, i, Noor::g_renderer_data->rad_prefilter_shader);
			}
		}
	}
	else if (key == NOOR_KEY_E && action == NOOR_PRESSED)
	{
		if(Noor::recompile_shader(Noor::g_renderer_data->irrad_conv_shader))
		{
			Noor::bind_cubemap(cubemap, 0);
			Noor::bind_shader(Noor::g_renderer_data->irrad_conv_shader);
			Noor::set_shader_uniform_int(Noor::g_renderer_data->irrad_conv_shader, "u_envmap", 0);
			Noor::render_to_cubemap(irrad_conv_cubemap, 0, Noor::g_renderer_data->irrad_conv_shader);
		}
	}
}

void mouse_button_callback(int32_t button, int32_t action, int32_t mods)
{

}

void mouse_position_callback(int32_t x, int32_t y)
{
	camera_controller.mouse_position_event(x, y);
}

// WRITE A Noor::shutdown
void shutdown()
{
	Noor::delete_texture(texture);

#if RENDER_MODEL
	for(int i = 0; i < c_MODEL_COUNT; i++)
		Noor::delete_model(models[i]);
#endif
}
