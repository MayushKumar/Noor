#pragma once

#include "window.h"

#include <cstdint>

namespace Noor {

	struct Init
	{
		void* pWindowHandle;
		uint32_t Width, Height;
	};

	void init(Window::WindowHandle windowHandle);
	
}
