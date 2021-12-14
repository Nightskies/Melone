#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Melone
{
	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	public:
		static void init();

		static std::shared_ptr<spdlog::logger>& getCoreLogger() { return sCoreLogger; }
		static std::shared_ptr<spdlog::logger>& getClientLogger() { return sClientLogger; }
	};
}

#define MELONE_CORE_TRACE(...) ::Melone::Log::getCoreLogger()->trace(__VA_ARGS__)
#define MELONE_CORE_INFO(...) ::Melone::Log::getCoreLogger()->info(__VA_ARGS__)
#define MELONE_CORE_WARN(...) ::Melone::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MELONE_CORE_ERROR(...) ::Melone::Log::getCoreLogger()->error(__VA_ARGS__)
#define MELONE_CORE_FATAL(...) ::Melone::Log::getCoreLogger()->fatal(__VA_ARGS__)

#define MELONE_TRACE(...) ::Melone::Log::getClientLogger()->trace(__VA_ARGS__)
#define MELONE_INFO(...) ::Melone::Log::getClientLogger()->info(__VA_ARGS__)
#define MELONE_WARN(...) ::Melone::Log::getClientLogger()->warn(__VA_ARGS__)
#define MELONE_ERROR(...) ::Melone::Log::getClientLogger()->error(__VA_ARGS__)
#define MELONE_FATAL(...) ::Melone::Log::getClientLogger()->fatal(__VA_ARGS__)

