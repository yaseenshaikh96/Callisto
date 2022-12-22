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

#ifdef CALLISTO_ENABLE_ASSERTS
	#define CALLISTO_ASSERT(x, ...) { if(!(x)) { CALLISTO_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define CALLISTO_CORE_ASSERT(x, ...) { if(!(x)) { CALLISTO_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CALLISTO_ASSERT(X, ...	)
	#define CALLISTO_CORE_ASSERT(X, ...)
#endif

#define BIT(x) (1 << x)