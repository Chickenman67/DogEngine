#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

// fmt v10+ requires explicit formatter specialization for custom types
template<>
struct fmt::formatter<DogEngine::Event> : fmt::ostream_formatter {};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

namespace DogEngine {
	class DOGENGINE_API Log
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

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//Log macros for the core
#define GE_CORE_TRACE(...)::DogEngine::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define GE_CORE_WARN(...)::DogEngine::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define GE_CORE_INFO(...)::DogEngine::Log::GetCoreLogger()->info(__VA_ARGS__);
#define GE_CORE_ERROR(...)::DogEngine::Log::GetCoreLogger()->error(__VA_ARGS__);
#define GE_CORE_FATAL(...)::DogEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__);

//log macros for the client
#define GE_TRACE(...)::DogEngine::Log::GetClientLogger()->trace(__VA_ARGS__);
#define GE_WARN(...)::DogEngine::Log::GetClientLogger()->warn(__VA_ARGS__);
#define GE_INFO(...)::DogEngine::Log::GetClientLogger()->info(__VA_ARGS__);
#define GE_ERROR(...)::DogEngine::Log::GetClientLogger()->error(__VA_ARGS__);
#define GE_FATAL(...)::DogEngine::Log::GetClientLogger()->fatal(__VA_ARGS__);
