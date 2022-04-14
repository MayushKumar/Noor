#pragma once

#include <memory.h>

#include <spdlog/spdlog.h>

namespace Noor
{
	namespace Log
	{
		extern std::shared_ptr<spdlog::logger> spClientLogger;
		extern std::shared_ptr<spdlog::logger> spCoreLogger;
		
		void init();
	}

#define NOOR_TRACE(...)	Log::spClientLogger->trace(__VA_ARGS__)
#define NOOR_INFO(...)	Log::spClientLogger->info(__VA_ARGS__)
#define NOOR_WARN(...)	Log::spClientLogger->warn(__VA_ARGS__)
#define NOOR_ERROR(...)	Log::spClientLogger->error(__VA_ARGS__)

#define NOOR_CORE_TRACE(...) Log::spCoreLogger->trace(__VA_ARGS__)
#define NOOR_CORE_INFO(...)	Log::spCoreLogger->info(__VA_ARGS__)
#define NOOR_CORE_WARN(...)	Log::spCoreLogger->warn(__VA_ARGS__)
#define NOOR_CORE_ERROR(...) Log::spCoreLogger->error(__VA_ARGS__)

}    
