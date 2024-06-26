#pragma once
#include "defs.h"

#include <glm/glm.hpp>

#include "texture.h"

namespace Noor
{

	struct Framebuffer
	{
		uint32_t id;

		// Ref<Texture2D> color_buffer;
	};
	
	Ref<Framebuffer> create_framebuffer();
	void attach_color_texture2D(Ref<Framebuffer> framebuffer, Ref<Texture2D> texture, uint32_t index, uint32_t level);
	void attach_depth_texture(Ref<Framebuffer> framebuffer, Ref<Texture2D> texture);
	void attach_color_cubemap_face(Ref<Framebuffer> framebuffer, Ref<CubeMap> cubemap, uint32_t index, uint32_t face, uint32_t level);
	void bind_framebuffer(Ref<Framebuffer> framebuffer);

	void push_framebuffer(Ref<Framebuffer> framebuffer);
	void pop_framebuffer();
	
	void set_clear_color(glm::vec4 color);
	void clear_color_buffer();
	void clear_color_buffer(Ref<Framebuffer> framebuffer, uint32_t index);
	void clear_depth_buffer();
	void clear_depth_buffer(Ref<Framebuffer> framebuffer);
}    
