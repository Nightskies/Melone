#include "mlpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Melone
{
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		mCoreLogger = spdlog::stdout_color_mt("MELONE");
		mCoreLogger->set_level(spdlog::level::trace);

		mClientLogger = spdlog::stdout_color_mt("CLIENT");
		mClientLogger->set_level(spdlog::level::trace);
	}
}
