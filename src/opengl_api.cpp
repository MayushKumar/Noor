#include "core.h"
#include "graphics_api.h"

#include "buffers.h"
#include "log.h"

#include <stack>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Noor
{

	static std::stack<glm::u32vec2> gs_viewport_stack;

	namespace GraphicsAPI
	{


		void opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
								   const GLchar *message, const void *userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				NOOR_CORE_TRACE(message);
				break;
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

			NOOR_CORE_TRACE("Graphics Vendor: {0}\n\t\t\tRenderer: {1}\n\t\t\tOpenGL Version: {2}\n\t\t\tGLSL Version: {3}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, 0);
			glDebugMessageCallback((GLDEBUGPROC)opengl_debug_callback, nullptr);

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		}

	}

	// void set_viewport(uint32_t width, uint32_t height)
	// {
	// 	glViewport(0, 0, width, height);
	// }

	void push_viewport(uint32_t width, uint32_t height)
	{
		gs_viewport_stack.push(glm::u32vec2(width, height));
		glViewport(0, 0, width, height);
	}

	void pop_viewport()
	{
		gs_viewport_stack.pop();
		glViewport(0, 0, gs_viewport_stack.top().x, gs_viewport_stack.top().y);
	}

	void set_backface_culling(bool enabled)
	{
		if(enabled)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}

	void set_depth_testing(bool enabled)
	{
		if(enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void set_depth_mask(bool enabled)
	{
		glDepthMask(enabled);
	}

}
