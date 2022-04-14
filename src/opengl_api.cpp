#include "graphics_api.h"
#include "core.h"

#include "buffers.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Noor
{
	namespace GraphicsAPI
	{

		void init(Window::WindowHandle windowHandle)
		{
			glfwMakeContextCurrent((GLFWwindow*)windowHandle);

			if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				NOOR_CORE_ERROR("Could not initialize glad!");
			}

		}

	}
}    
