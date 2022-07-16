#pragma once

#include "core.h"

#include "util.h"

namespace Noor
{

	struct Texture2DProps
	{
		uint32_t mip_count;
		uint32_t internal_format;

		uint32_t min_filter;
		uint32_t mag_filter;
		uint32_t wrapping;
	};

	struct Texture2D
	{
		uint32_t id;

		Texture2DProps props;
	};

	Ref<Texture2D> create_texture(Texture2DProps props, Util::Image image);
	void bind_texture(Ref<Texture2D> texture, uint32_t slot);
	
}
