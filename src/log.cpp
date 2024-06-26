#include "log.h"

namespace Noor
{
	namespace Log
	{

		std::shared_ptr<spdlog::logger> spClientLogger;
		std::shared_ptr<spdlog::logger> spCoreLogger;

		void init()
		{
			spdlog::set_pattern("%^[%T] [%n] :: %v%$");
			spClientLogger = spdlog::stdout_color_mt("CLIENT");
			spClientLogger->set_level(spdlog::level::trace);
			spCoreLogger = spdlog::stdout_color_mt("NOOR");
			spCoreLogger->set_level(spdlog::level::trace);
		}
		
	}

}    
