#pragma once

#include "defs.h"

#include "util.h"

namespace Noor
{

	struct Texture2DProps
	{
		uint32_t width;
		uint32_t height;

		uint32_t internal_format;
		uint32_t mip_count;

		uint32_t min_filter;
		uint32_t mag_filter;
		uint32_t wrapping;
	};

	struct Texture2D
	{
		uint32_t id;

		Texture2DProps props;
	};

	struct CubeMapProps
	{
		uint32_t dimension;
		
		uint32_t internal_format;
		uint32_t mip_count;

		uint32_t min_filter;
		uint32_t mag_filter;
		uint32_t wrapping;
	};

	struct CubeMap
	{
		uint32_t id;

		CubeMapProps props;
	};

	Ref<Texture2D> create_texture(Texture2DProps props);
	Ref<Texture2D> create_texture(Texture2DProps props, Util::Image image);
	void bind_texture(Ref<Texture2D> texture, uint32_t slot);
	void gen_mipmaps(Ref<Texture2D> texture);
	Util::Image get_texture_image(Ref<Texture2D> texture, uint32_t level);
	void delete_texture(Ref<Texture2D> texture);

	Ref<CubeMap> create_cubemap(CubeMapProps props);
	void bind_cubemap(Ref<CubeMap> cubemap, uint32_t slot);
	void gen_mipmaps(Ref<CubeMap> cubemap);
	void delete_cubemap(Ref<CubeMap> cubemap);
	
}
