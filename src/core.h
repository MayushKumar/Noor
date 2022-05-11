#pragma once

#include <memory.h>

#include "log.h"
#include "datatypes.h"
#include "input_defs.h"

namespace Noor
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args) ...);
	}
}


