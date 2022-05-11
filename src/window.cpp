#include "window.h"

#include <GLFW/glfw3.h>

namespace Noor
{

	namespace Window
	{
		std::unordered_map<WindowHandle, Ref<WindowProps>> windowMap;	

		WindowHandle create_window(WindowProps window_props)
		{
			glfwInit();
			glfwWindowHint(GLFW_RESIZABLE, true);
			GLFWwindow* window_handle = glfwCreateWindow(window_props.width, window_props.height, "Noor", nullptr, nullptr);
			Ref<WindowProps> window_props_ref = CreateRef<WindowProps>(window_props);
			glfwSetWindowUserPointer(window_handle, window_props_ref.get());
			windowMap.emplace((WindowHandle)window_handle, window_props_ref);

			glfwSetKeyCallback(window_handle,
							   [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
								   WindowProps* props = (WindowProps*)glfwGetWindowUserPointer(window);
								   props->key_callback_fn(key, action, mods);
							   });

			return (WindowHandle)window_handle;
		}

		void poll_events()
		{
			glfwPollEvents();
		}

		void swap_buffers(WindowHandle window)
		{
			glfwSwapBuffers((GLFWwindow*)window);
		}

		void set_VSync(WindowHandle window, bool state)
		{
			glfwSwapInterval(!state);
		}

		bool window_should_close(WindowHandle window)
		{
			return glfwWindowShouldClose((GLFWwindow*)window);
		}			

	}
}    
