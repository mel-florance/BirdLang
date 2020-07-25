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

<<<<<<< Updated upstream
#define PI 3.141592653589793f
=======
#define PI 3.14159265358f
>>>>>>> Stashed changes
#define TAU 2.0f * PI
