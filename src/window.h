#pragma once

#include "core.h"

#include <unordered_map>
#include <cstdint>

namespace Noor
{

	namespace Window
	{
		using WindowHandle = void*;
		using KeyCallbackFn = void (*) (uint32_t key, uint32_t action, uint32_t mods);

		struct WindowProps
		{
			uint32_t width, height;
			KeyCallbackFn key_callback_fn;
		};

		extern std::unordered_map<WindowHandle, WindowProps> window_map;


		void* create_window(WindowProps window_props);
		void poll_events();
		void swap_buffers(WindowHandle window);
		void set_VSync(WindowHandle window, bool state);
		bool window_should_close(WindowHandle window);
	}
	
}    
