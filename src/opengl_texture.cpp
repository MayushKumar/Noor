#include "texture.h"

#include <glad/glad.h>

namespace Noor
{

	uint32_t internal_format_to_gl_base_format(uint32_t internal_format);
	uint32_t internal_format_to_gl_data_type(uint32_t internal_format);

	Ref<Texture2D> create_texture(Texture2DProps props, Util::Image image)
	{
		Ref<Texture2D> texture = CreateRef<Texture2D>();
		texture->props = props;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &texture->id);
		glTextureStorage2D(texture->id, props.mip_count, props.internal_format, image.width, image.height);

		glTextureSubImage2D(texture->id, 0, 0, 0, image.width, image.height,
							internal_format_to_gl_base_format(props.internal_format),
							internal_format_to_gl_data_type(props.internal_format), image.data);

		glGenerateTextureMipmap(texture->id);

		glTextureParameteri(texture->id, GL_TEXTURE_MAG_FILTER, props.mag_filter);
		glTextureParameteri(texture->id, GL_TEXTURE_MIN_FILTER, props.min_filter);
		glTextureParameteri(texture->id, GL_TEXTURE_WRAP_T, props.wrapping);
		glTextureParameteri(texture->id, GL_TEXTURE_WRAP_T, props.wrapping);
		
		return texture;
	}

	void bind_texture(Ref<Texture2D> texture, uint32_t slot)
	{
		glBindTextureUnit(slot, texture->id);
	}

	void delete_texture(Ref<Texture2D> texture)
	{
		glDeleteTextures(1, &texture->id);
	}

	uint32_t internal_format_to_gl_base_format(uint32_t internal_format)
	{
		switch(internal_format)
		{
		case NOOR_RGB8: return GL_RGB;
		case NOOR_RGBA8: return GL_RGBA;
		}

		return 0;
	}
	
	uint32_t internal_format_to_gl_data_type(uint32_t internal_format)
	{
		switch(internal_format)
		{
		case NOOR_RGB8:
		case NOOR_RGBA8: return GL_UNSIGNED_BYTE;
		}

		return 0;
	}
}    
