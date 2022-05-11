#include <iostream>

#include "Noor.h"

Noor::Window::WindowProps window_props;
Noor::Window::WindowHandle window_handle;

void game_loop();
void key_callback(uint32_t key, uint32_t action, uint32_t mods);

Noor::Ref<Noor::VertexBuffer> vertex_buffer;
Noor::Ref<Noor::IndexBuffer> index_buffer;
Noor::Ref<Noor::VertexArray> vertex_array;
Noor::Ref<Noor::Shader> shader;

int main()
{
	window_props.width = 1280;
	window_props.height = 720;
	window_props.key_callback_fn = key_callback;
	window_handle = Noor::Window::create_window(window_props);
	Noor::init(window_handle);

	Noor::set_viewport(1280, 720);

	float vertices [3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	uint32_t indices[3] = {0, 1, 2};

	vertex_buffer = Noor::create_vertex_buffer((void*)vertices, sizeof(vertices),
											   {{"Positions", NOOR_FLOAT, 3, 0}});
	index_buffer = Noor::create_index_buffer((void*)indices, sizeof(indices), NOOR_UINT);
	vertex_array = Noor::create_vertex_array();
	Noor::attach_vertex_buffer(vertex_array, vertex_buffer, 0);
	Noor::attach_index_buffer(vertex_array, index_buffer);

	shader = Noor::create_shader_from_files("assets/shaders/test_vs.glsl", "assets/shaders/test_fs.glsl");
	Noor::bind_shader(shader);

	game_loop();
}

void game_loop()
{
	while(!Noor::Window::window_should_close(window_handle))
	{
		Noor::Window::poll_events();
		Noor::Window::swap_buffers(window_handle);
		Noor::set_clear_color({0.05f, 0.05f, 0.05f});

		Noor::clear_color_buffer();
		Noor::draw_indexed(vertex_array);
	}
}

void key_callback(uint32_t key, uint32_t action, uint32_t mods)
{
	if (key == NOOR_KEY_W && action == NOOR_PRESSED)
	{
		NOOR_TRACE("'W' pressed");
	} else if (key == NOOR_KEY_W && action == NOOR_RELEASED)
	{
		NOOR_TRACE("'W' released");
	}
}
