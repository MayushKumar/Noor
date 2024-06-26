#include "framebuffer.h"

#include <stack>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Noor
{

	static std::stack<Ref<Framebuffer>> gs_framebuffer_stack;
	static float gs_clear_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	static float gs_clear_depth_val = 1.0f;

	Ref<Framebuffer> create_framebuffer()
	{
		Ref<Framebuffer> framebuffer = CreateRef<Framebuffer>();
		glCreateFramebuffers(1, &framebuffer->id);
		return framebuffer;
	}

	void attach_color_texture2D(Ref<Framebuffer> framebuffer, Ref<Texture2D> texture, uint32_t index, uint32_t level)
	{
		glNamedFramebufferTexture(framebuffer->id, GL_COLOR_ATTACHMENT0 + index, texture->id, level);
	}

	void attach_depth_texture(Ref<Framebuffer> framebuffer, Ref<Texture2D> texture)
	{
		glNamedFramebufferTexture(framebuffer->id, GL_DEPTH_ATTACHMENT, texture->id, 0);
	}

	void attach_color_cubemap_face(Ref<Framebuffer> framebuffer, Ref<CubeMap> cubemap, uint32_t index, uint32_t face, uint32_t level)
	{
		glNamedFramebufferTextureLayer(framebuffer->id, GL_COLOR_ATTACHMENT0 + index, cubemap->id, level, face);
	}

	void bind_framebuffer(const Ref<Framebuffer> framebuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->id);
	}

	void push_framebuffer(const Ref<Framebuffer> framebuffer)
	{
		bind_framebuffer(framebuffer);
		gs_framebuffer_stack.push(framebuffer);
	}

	void pop_framebuffer()
	{
		gs_framebuffer_stack.pop();
		if (gs_framebuffer_stack.size() == 0)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		else
			glBindFramebuffer(GL_FRAMEBUFFER, gs_framebuffer_stack.top()->id);
	}


	// Clearing the frambuffer

	void set_clear_color(glm::vec4 color)
	{
		std::memcpy(gs_clear_color, glm::value_ptr(color), sizeof(gs_clear_color));
	}

	void clear_color_buffer()
	{
		glClearNamedFramebufferfv(0, GL_COLOR, 0, gs_clear_color);
	}

	void clear_color_buffer(const Ref<Framebuffer> framebuffer, uint32_t index)
	{
		glClearNamedFramebufferfv(framebuffer->id, GL_COLOR, index, gs_clear_color);
	}

	void clear_depth_buffer()
	{
		glClearNamedFramebufferfv(0, GL_DEPTH, 0, &gs_clear_depth_val);
	}

	void clear_depth_buffer(const Ref<Framebuffer> framebuffer)
	{
		glClearNamedFramebufferfv(framebuffer->id, GL_DEPTH, 0, &gs_clear_depth_val);
	}

}
