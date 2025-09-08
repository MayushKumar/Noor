#include "window.h"

#include "defs.h"

#include <GLFW/glfw3.h>

namespace Noor
{

	namespace Window
	{
		std::unordered_map<WindowHandle, Ref<WindowProps>> windowMap;	

		WindowHandle create_window(WindowProps window_props)
		{
			glfwInitHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

			glfwInit();

			glfwWindowHint(GLFW_RESIZABLE, true);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

			GLFWwindow* window_handle = glfwCreateWindow(window_props.width, window_props.height, "Noor", nullptr, nullptr);
			Ref<WindowProps> window_props_ref = CreateRef<WindowProps>(window_props);
			glfwSetWindowUserPointer(window_handle, window_props_ref.get());
			windowMap.emplace((WindowHandle)window_handle, window_props_ref);

			glfwSetKeyCallback(window_handle,
							   [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
								   WindowProps* props = (WindowProps*)glfwGetWindowUserPointer(window);
								   props->key_callback_fn(key, action, mods);
							   });
			glfwSetMouseButtonCallback(window_handle,
									   [](GLFWwindow* window, int button,int action, int mods){
										   WindowProps* props = (WindowProps*)glfwGetWindowUserPointer(window);
										   props->mouse_button_callback_fn(button, action, mods);
									   });
			glfwSetCursorPosCallback((GLFWwindow*)window_handle,
									 [](GLFWwindow* window, double x_pos, double y_pos){
										 WindowProps* props = (WindowProps*)glfwGetWindowUserPointer(window);
										 props->mouse_position_callback_fn(x_pos, y_pos);
									 });
			// for(int i = 0; i < 5; i++)
			// {
			// 	NOOR_CORE_INFO("{0} : {1}", i + 1, glfwJoystickPresent(GLFW_JOYSTICK_1 + i));
			// 	// NOOR_CORE_INFO("{0}", i + 1);
			// }

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
			glfwSwapInterval(state);
		}

		void set_cursor_enabled(WindowHandle window, bool enabled)
		{
			enabled ? glfwSetInputMode((GLFWwindow*)window, GLFW_CURSOR, GLFW_CURSOR_NORMAL)
				    : glfwSetInputMode((GLFWwindow*)window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		bool window_should_close(WindowHandle window)
		{
			return glfwWindowShouldClose((GLFWwindow*)window);
		}			

		bool is_key_pressed(WindowHandle window, uint32_t key)
		{
			return glfwGetKey((GLFWwindow*)window, key) == GLFW_PRESS;
		}

		void get_cursor_position(WindowHandle window, int32_t &x, int32_t &y)
		{
			double x_pos, y_pos;
			glfwGetCursorPos((GLFWwindow*)window, &x_pos, &y_pos);
			x = x_pos;
			y = y_pos;
		}

		unsigned char* get_joystick_buttons(int& count)
		{
			unsigned char* ptr;
			glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
			return ptr;
		}

		float* get_joystick_axes(int& count)
		{
			float* ptr;
			glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
			return ptr;
		}

	}
}    
