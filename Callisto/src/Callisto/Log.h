#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace Callisto
{
	class CALLISTO_API Log
	{
	public: 
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }
	 
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//core logs
#define CALLISTO_CORE_ERROR(...)		::Callisto::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CALLISTO_CORE_CRITICAL(...)		::Callisto::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CALLISTO_CORE_WARN(...)			::Callisto::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CALLISTO_CORE_INFO(...)			::Callisto::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CALLISTO_CORE_TRACE(...)		::Callisto::Log::GetCoreLogger()->trace(__VA_ARGS__)

//client logs											  
#define CALLISTO_ERROR(...)				::Callisto::Log::GetClientLogger()->error(__VA_ARGS__)
#define CALLISTO_CRITICAL(...)			::Callisto::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CALLISTO_WARN(...)				::Callisto::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CALLISTO_INFO(...)				::Callisto::Log::GetClientLogger()->info(__VA_ARGS__)
#define CALLISTO_TRACE(...)				::Callisto::Log::GetClientLogger()->trace(__VA_ARGS__)
