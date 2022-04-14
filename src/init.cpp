#include "init.h"

#include "window.h"
#include "graphics_api.h"
#include "log.h"

namespace Noor
{

	Window::WindowHandle windowHandle;
	void gameLoop();

	void init(Window::WindowHandle windowHandle)
	{
		Noor::windowHandle = windowHandle;
		Log::init();
		GraphicsAPI::init(windowHandle);
	}

}
