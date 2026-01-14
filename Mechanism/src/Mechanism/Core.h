#pragma once

#ifdef MECHANISM_PLATFORM_WINDOWS

	#ifdef MECHANISM_BUILD_DLL
		#define MECHANISM_API __declspec(dllexport)
	#else
		#define MECHANISM_API __declspec(dllimport)
	#endif

#else 
#error Mechanism only supports Windows!
#endif
