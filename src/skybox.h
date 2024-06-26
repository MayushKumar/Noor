#pragma once
#include "defs.h"

#include "texture.h"
#include "shader.h"

namespace Noor {

	void render_to_cubemap(Ref<CubeMap> cubemap, uint32_t level, Ref<Shader> shader);
	
}
