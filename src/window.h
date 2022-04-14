#pragma once

#include <unordered_map>
#include <cstdint>

namespace Noor
{

	namespace Window
	{
		typedef void* WindowHandle;

		struct WindowProps
		{
			uint32_t width, height;
		};

		extern std::unordered_map<WindowHandle, WindowProps> windowMap;

		void* createWindow(WindowProps windowProps);
		void pollEvents();
		void swapBuffers(WindowHandle window);
		void setVSync(WindowHandle window, bool state);
		bool windowShouldClose(WindowHandle window);
	}
	
}    
