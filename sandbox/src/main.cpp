#include <iostream>

#include "Noor.h"

#include "camera_controller.h"

Noor::Window::WindowProps window_props;
Noor::Window::WindowHandle window_handle;

void game_loop();
void shutdown();
void key_callback(int32_t key, int32_t action, int32_t mods);
void mouse_button_callback(int32_t button, int32_t action, int32_t mods);
void mouse_position_callback(int32_t x, int32_t y);

Noor::Ref<Noor::VertexBuffer> vertex_buffer;
Noor::Ref<Noor::IndexBuffer> index_buffer;
Noor::Ref<Noor::VertexArray> vertex_array;
Noor::Ref<Noor::Texture2D> texture;
Noor::Ref<Noor::Shader> shader;

static const uint32_t c_WINDOW_WIDTH = 1280, c_WINDOW_HEIGHT = 720;

CameraController camera_controller;
int main()
{
	window_props.width = c_WINDOW_WIDTH;
	window_props.height = c_WINDOW_HEIGHT;
	window_props.key_callback_fn = key_callback;
	window_props.mouse_button_callback_fn = mouse_button_callback;
	window_props.mouse_position_callback_fn = mouse_position_callback;
	window_handle = Noor::Window::create_window(window_props);
	Noor::init(window_handle);

	camera_controller = CameraController(window_handle, glm::radians(60.0f),
										 (float)c_WINDOW_WIDTH / (float)c_WINDOW_HEIGHT, 0.1f, 100.0f);

	float vertices [4 * (3 + 2)] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f
	};

	uint32_t indices[6] = {0, 1, 2, 1, 3, 2};

	vertex_buffer = Noor::create_vertex_buffer((void*)vertices, sizeof(vertices),
											   {{"Positions", NOOR_FLOAT, 3, 0},
												{"TexCoords", NOOR_FLOAT, 2, 3 * sizeof(float)}});
	index_buffer = Noor::create_index_buffer((void*)indices, sizeof(indices), NOOR_UINT);
	vertex_array = Noor::create_vertex_array();
	Noor::attach_vertex_buffer(vertex_array, vertex_buffer, 0);
	Noor::attach_index_buffer(vertex_array, index_buffer);

	shader = Noor::create_shader_from_files("assets/shaders/test_vs.glsl", "assets/shaders/test_fs.glsl");
	Noor::bind_shader(shader);

	Noor::Util::Image image = Noor::Util::load_image("assets/textures/texture.png");
	Noor::Texture2DProps texture_props;
	texture_props.internal_format = NOOR_RGBA8;
	texture_props.mip_count = 1;
	texture_props.min_filter = NOOR_LINEAR;
	texture_props.mag_filter = NOOR_NEAREST;
	texture_props.wrapping = NOOR_REPEAT;
	texture = Noor::create_texture(texture_props, image);

	game_loop();

	shutdown();
}

void game_loop()
{
	Noor::bind_texture(texture, 1);
	Noor::set_shader_uniform_int(shader, "u_texture", 1);
	while(!Noor::Window::window_should_close(window_handle))
	{
		Noor::Window::poll_events();

		camera_controller.update();

		Noor::Window::swap_buffers(window_handle);
		Noor::set_clear_color({0.05f, 0.05f, 0.05f});
		Noor::clear_color_buffer();

		Noor::set_shader_uniform_mat4(shader, "u_view_proj_mat", camera_controller.get_view_proj_matrix());
		Noor::draw_indexed(vertex_array);
	}
}

void key_callback(int32_t key, int32_t action, int32_t mods)
{
	camera_controller.key_event(key, action, mods);

	if (key == NOOR_KEY_X && action == NOOR_PRESSED)
	{
		camera_controller.set_enabled(!camera_controller.is_enabled());
	}
}

void mouse_button_callback(int32_t button, int32_t action, int32_t mods)
{

}

void mouse_position_callback(int32_t x, int32_t y)
{
	camera_controller.mouse_position_event(x, y);
}

void shutdown()
{
	Noor::delete_vertex_buffer(vertex_buffer);
	Noor::delete_index_buffer(index_buffer);
	Noor::delete_vertex_array(vertex_array);
}
