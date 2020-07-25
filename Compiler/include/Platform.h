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

#define PI 3.14159265358979323846264338327950288419716939937
#define TAU 2.0 * PI
