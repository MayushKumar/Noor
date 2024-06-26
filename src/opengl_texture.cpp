#include "defs.h"
#include "texture.h"
#include "util.h"

#include <cstdlib>

#include <glad/glad.h>

namespace Noor
{

	uint32_t internal_format_to_channels(uint32_t internal_format);
	uint32_t internal_format_to_type(uint32_t internal_format);
	uint32_t internal_format_to_gl_base_format(uint32_t internal_format);
	uint32_t internal_format_to_gl_data_type(uint32_t internal_format);

	Ref<Texture2D> create_texture(Texture2DProps props)
	{
		Ref<Texture2D> texture = CreateRef<Texture2D>();
		texture->props = props;

		glCreateTextures(GL_TEXTURE_2D, 1, &texture->id);
		glTextureStorage2D(texture->id, props.mip_count + 1, props.internal_format, props.width, props.height);

		glTextureParameteri(texture->id, GL_TEXTURE_MAG_FILTER, props.mag_filter);
		glTextureParameteri(texture->id, GL_TEXTURE_MIN_FILTER, props.min_filter);
		glTextureParameteri(texture->id, GL_TEXTURE_WRAP_T, props.wrapping);
		glTextureParameteri(texture->id, GL_TEXTURE_WRAP_S, props.wrapping);

		return texture;
	}

	Ref<Texture2D> create_texture(Texture2DProps props, Util::Image image)
	{
		Ref<Texture2D> texture = CreateRef<Texture2D>();
		texture->props = props;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &texture->id);
		glTextureStorage2D(texture->id, props.mip_count + 1, props.internal_format, image.width, image.height);

		glTextureSubImage2D(texture->id, 0, 0, 0, image.width, image.height,
							internal_format_to_gl_base_format(props.internal_format),
							internal_format_to_gl_data_type(props.internal_format), image.data);

		glTextureParameteri(texture->id, GL_TEXTURE_MAG_FILTER, props.mag_filter);
		glTextureParameteri(texture->id, GL_TEXTURE_MIN_FILTER, props.min_filter);
		glTextureParameteri(texture->id, GL_TEXTURE_WRAP_T, props.wrapping);
		glTextureParameteri(texture->id, GL_TEXTURE_WRAP_S, props.wrapping);
		
		return texture;
	}

	Ref<CubeMap> create_cubemap(CubeMapProps props)
	{
		Ref<CubeMap> cubemap = CreateRef<CubeMap>();
		cubemap->props = props;

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &cubemap->id);
		glTextureStorage2D(cubemap->id, props.mip_count + 1, props.internal_format, props.dimension, props.dimension);
		
		glTextureParameteri(cubemap->id, GL_TEXTURE_MAG_FILTER, props.mag_filter);
		glTextureParameteri(cubemap->id, GL_TEXTURE_MIN_FILTER, props.min_filter);
		glTextureParameteri(cubemap->id, GL_TEXTURE_WRAP_T, props.wrapping);
		glTextureParameteri(cubemap->id, GL_TEXTURE_WRAP_S, props.wrapping);

		return cubemap;
	}

	void bind_texture(Ref<Texture2D> texture, uint32_t slot)
	{
		glBindTextureUnit(slot, texture->id);
	}

	void bind_cubemap(Ref<CubeMap> cubemap, uint32_t slot)
	{
		glBindTextureUnit(slot, cubemap->id);
	}

	void gen_mipmaps(Ref<Texture2D> texture)
	{
		glGenerateTextureMipmap(texture->id);
	}

	void gen_mipmaps(Ref<CubeMap> cubemap)
	{
		glGenerateTextureMipmap(cubemap->id);
	}

	Util::Image get_texture_image(Ref<Texture2D> texture, uint32_t level)
	{
		Util::Image image;
		image.width = texture->props.width;
		image.height = texture->props.height;
		uint32_t internal_format = texture->props.internal_format;
		image.type = internal_format_to_type(internal_format);
		image.channels = internal_format_to_channels(internal_format);
		uint32_t size = data_type_size(image.type) * image.width * image.height * image.channels;
		image.data = std::malloc(size);
		glGetTextureImage(texture->id, level,
						  internal_format_to_gl_base_format(internal_format),
						  internal_format_to_gl_data_type(internal_format),
						  size, image.data);
		return image;
	}

	void delete_texture(Ref<Texture2D> texture)
	{
		glDeleteTextures(1, &texture->id);
	}

	void delete_cubemap(Ref<CubeMap> cubemap)
	{
		glDeleteTextures(1, &cubemap->id);
	}

	uint32_t internal_format_to_channels(uint32_t internal_format)
	{
		switch(internal_format)
		{
		case NOOR_RG16F: return 2;

		case NOOR_RGB8: 
		case NOOR_SRGB8:
		case NOOR_RGB16F: return 3;

		case NOOR_RGBA8: return 4;
		}

		return 0;
	}

	uint32_t internal_format_to_type(uint32_t internal_format)
	{
		return internal_format_to_gl_data_type(internal_format);
	}

	uint32_t internal_format_to_gl_base_format(uint32_t internal_format)
	{
		switch(internal_format)
		{
		case NOOR_RG16F: return GL_RG;

		case NOOR_RGB8: 
		case NOOR_SRGB8:
		case NOOR_RGB32F: 
		case NOOR_RGB16F: return GL_RGB;

		case NOOR_RGBA8: return GL_RGBA;
		}

		return 0;
	}
	
	uint32_t internal_format_to_gl_data_type(uint32_t internal_format)
	{
		switch(internal_format)
		{
		case NOOR_RGB8:
		case NOOR_SRGB8:
		case NOOR_RGBA8: return GL_UNSIGNED_BYTE;

		case NOOR_RG16F:
		case NOOR_RGB16F: return GL_HALF_FLOAT;

		case NOOR_RGB32F: return GL_FLOAT;
		}

		return 0;
	}
}    
