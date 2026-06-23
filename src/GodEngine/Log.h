#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GodEngine {
	class GODENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;


	};

}
//Log macros for the core
#define GE_CORE_TRACE(...)::GodEngine::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define GE_CORE_WARN(...)::GodEngine::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define GE_CORE_INFO(...)::GodEngine::Log::GetCoreLogger()->info(__VA_ARGS__);
#define GE_CORE_ERROR(...)::GodEngine::Log::GetCoreLogger()->error(__VA_ARGS__);
#define GE_CORE_FATAL(...)::GodEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__);

//log macros for the client
#define GE_TRACE(...)::GodEngine::Log::GetClientLogger()->trace(__VA_ARGS__);
#define GE_WARN(...)::GodEngine::Log::GetClientLogger()->warn(__VA_ARGS__);
#define GE_INFO(...)::GodEngine::Log::GetClientLogger()->info(__VA_ARGS__);
#define GE_ERROR(...)::GodEngine::Log::GetClientLogger()->error(__VA_ARGS__);
#define GE_FATAL(...)::GodEngine::Log::GetClientLogger()->fatal(__VA_ARGS__);




