#pragma once

#include <unordered_map>
#include <cstdint>

namespace Noor
{

	namespace Window
	{
		using WindowHandle = void*;
		using KeyCallbackFn = void (*)(int32_t key, int32_t action, int32_t mods);
		using MouseButtonCallbackFn = void (*)(int32_t button, int32_t action, int32_t mods);
		using MousePositionCallbackFn = void (*)(int32_t x, int32_t y);

		struct WindowProps
		{
			uint32_t width, height;
			KeyCallbackFn key_callback_fn;
			MouseButtonCallbackFn mouse_button_callback_fn;
			MousePositionCallbackFn mouse_position_callback_fn;
		};

		extern std::unordered_map<WindowHandle, WindowProps> window_map;


		WindowHandle create_window(WindowProps window_props);
		void poll_events();
		void swap_buffers(WindowHandle window);
		void set_VSync(WindowHandle window, bool state);
		void set_cursor_enabled(WindowHandle window, bool enabled);

		bool window_should_close(WindowHandle window);
		bool is_key_pressed(uint32_t key);
		void get_cursor_position(WindowHandle window, int32_t &x, int32_t &y);
		unsigned char* get_joystick_buttons(int& count);
		float* get_joystick_axes(int& count);
	}
	
}    
