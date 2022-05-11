#include "init.h"

#include "window.h"
#include "graphics_api.h"
#include "log.h"

namespace Noor
{

	Window::WindowHandle window_handle;
	void gameLoop();

	void init(Window::WindowHandle windowHandle)
	{
		Noor::window_handle = windowHandle;
		Log::init();
		GraphicsAPI::init(window_handle);
	}

}
