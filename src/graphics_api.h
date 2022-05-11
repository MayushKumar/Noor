#include "window.h"

#include <glm/glm.hpp>

namespace Noor
{

	namespace GraphicsAPI
	{

		void init(Window::WindowHandle windowHandle);

	}

	void set_clear_color(glm::vec3 color);
	void clear_color_buffer();
	void set_viewport(uint32_t width, uint32_t height);

}    
