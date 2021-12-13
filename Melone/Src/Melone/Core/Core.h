#pragma once

/* Platform detection using predefined macros */
// Windows x64/x86
#ifdef _WIN32
	// Windows x64
	#ifdef _WIN64
		#define MELONE_PLATFORM_WINDOWS
	// Windows x86
	#else
		#error "x86 Builds are not supported!"
	#endif
// macOS
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define MELONE_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define MELONE_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
// Android
#elif defined(__ANDROID__)
	#define MELONE_PLATFORM_ANDROID
	#error "Android is not supported!"
// Linux
#elif defined(__linux__)
	#define MELONE_PLATFORM_LINUX
	#error "Linux is not supported!"
// Unknown platform
#else
	#error "Unknown platform!"
#endif