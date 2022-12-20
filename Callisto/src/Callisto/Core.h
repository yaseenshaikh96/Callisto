#pragma once

#ifdef CALLISTO_PLATFORM_WINDOWS

	#ifdef CALLISTO_BUILD_DLL
		#define CALLISTO_API __declspec(dllexport)
	#else
		#define CALLISTO_API __declspec(dllimport)
	#endif // CALLISTO_BUILD_DLL
#else
	#error Callisto only support windwos
#endif // CALLISTO_PLATFORM_WINDOWS
