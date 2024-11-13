#pragma once

#include "shader.h"

namespace Noor
{

	struct RenderPass
	{
		Ref<Shader> shader;
		std::vector<ShaderUniform> uniforms;
	};

}
