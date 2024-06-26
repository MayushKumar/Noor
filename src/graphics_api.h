#include "window.h"

#include <glm/glm.hpp>

namespace Noor
{

	namespace GraphicsAPI
	{

		void init(Window::WindowHandle windowHandle);

	}

	// void set_viewport(uint32_t width, uint32_t height);
	void push_viewport(uint32_t width, uint32_t height);
	void pop_viewport();
	void set_backface_culling(bool enabled);
	void set_depth_testing(bool enabled);
	void set_depth_mask(bool enabled);

}    
