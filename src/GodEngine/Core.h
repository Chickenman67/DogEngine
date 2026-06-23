#pragma once

#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_BUILD_DLL
		#define GODENGINE_API __declspec(dllexport)
	#else
		#define GODENGINE_API __declspec(dllimport)
	#endif
#else
	#error GodEngine is only on Windows
#endif

#define BIT(x) (1<<x)