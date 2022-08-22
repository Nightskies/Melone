#pragma once

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Melone
{
	class Log
	{
	private:
		static inline SPtr<spdlog::logger> CoreLogger;
		static inline SPtr<spdlog::logger> ClientLogger;

		Log() = default;
	public:
		static void Init();

		static SPtr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }
		static SPtr<spdlog::logger>& GetClientLogger() { return ClientLogger; }
	};
}

#define MELONE_CORE_TRACE(...) ::Melone::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MELONE_CORE_INFO(...) ::Melone::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MELONE_CORE_WARN(...) ::Melone::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MELONE_CORE_ERROR(...) ::Melone::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MELONE_CORE_FATAL(...) ::Melone::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define MELONE_TRACE(...) ::Melone::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MELONE_INFO(...) ::Melone::Log::GetClientLogger()->info(__VA_ARGS__)
#define MELONE_WARN(...) ::Melone::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MELONE_ERROR(...) ::Melone::Log::GetClientLogger()->error(__VA_ARGS__)
#define MELONE_FATAL(...) ::Melone::Log::GetClientLogger()->critical(__VA_ARGS__)

