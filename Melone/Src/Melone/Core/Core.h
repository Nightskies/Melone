#pragma once

#include <memory>

#include "PlatformDetection.h"

#ifdef MELONE_DEBUG
	#define MELONE_ENABLE_ASSERTS
#endif

#ifdef MELONE_ENABLE_ASSERTS
	#define MELONE_ASSERT(x, ...) { if(!(x)) { MELONE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MELONE_CORE_ASSERT(x, ...) { if(!(x)) { MELONE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MELONE_ASSERT(x, ...)
	#define MELONE_CORE_ASSERT(x, ...)
#endif

#define MELONE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)