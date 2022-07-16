#include "init.h"

#include "window.h"
#include "graphics_api.h"
#include "log.h"

namespace Noor
{

	void init(Window::WindowHandle window_handle)
	{
		Log::init();
		GraphicsAPI::init(window_handle);
	}

}
