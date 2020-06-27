#pragma once

#ifdef PLATFORM_WINDOWS
	#if DYNAMIC_LINK
		#ifdef BUILD_DLL
			#define API __declspec(dllexport)
		#else
			#define API __declspec(dllimport)
		#endif
	#else
		#define API
	#endif
#endif

#define PI 3.141592653589793f
#define TAU 2.0f * PI