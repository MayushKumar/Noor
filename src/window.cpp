#include "window.h"

#include <GLFW/glfw3.h>

namespace Noor
{

	namespace Window
	{
		std::unordered_map<WindowHandle, WindowProps> windowMap;	

		WindowHandle createWindow(WindowProps windowProps)
		{
			glfwInit();
			glfwWindowHint(GLFW_RESIZABLE, true);
			GLFWwindow* windowHandle = glfwCreateWindow(windowProps.width, windowProps.height, "Noor", nullptr, nullptr);
			windowMap.emplace((WindowHandle)windowHandle, windowProps);
			return (WindowHandle)windowHandle;
		}

		void pollEvents()
		{
			glfwPollEvents();
		}

		void swapBuffers(WindowHandle window)
		{
			glfwSwapBuffers((GLFWwindow*)window);
		}

		void setVSync(WindowHandle window, bool state)
		{
			glfwSwapInterval(!state);
		}

		bool windowShouldClose(WindowHandle window)
		{
			return glfwWindowShouldClose((GLFWwindow*)window);
		}			

	}
}    
