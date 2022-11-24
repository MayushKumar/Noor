#include "core.h"
#include "graphics_api.h"

#include "buffers.h"
#include "log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Noor
{

namespace GraphicsAPI
{

	void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
							   const GLchar *message, const void *userParam)
	{
		switch (severity)
		{
			// case GL_DEBUG_SEVERITY_NOTIFICATION:
			// 	NOOR_CORE_TRACE(message); break;
		case GL_DEBUG_SEVERITY_LOW:
			NOOR_CORE_INFO(message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			NOOR_CORE_WARN(message);
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			NOOR_CORE_ERROR(message);
			break;
		}
	}

	void init(Window::WindowHandle windowHandle)
	{
		glfwMakeContextCurrent((GLFWwindow *)windowHandle);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			NOOR_CORE_ERROR("Could not initialize glad!");
		}

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, 0);
		glDebugMessageCallback((GLDEBUGPROC)opengl_debug_callback, nullptr);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}

}

	void set_clear_color(glm::vec3 color)
	{
		glClearColor(color.r, color.g, color.b, 1.0f);
	}

	void clear_color_buffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void clear_depth_buffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void set_viewport(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void set_backface_culling(bool enabled)
	{
		if(enabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}

}
