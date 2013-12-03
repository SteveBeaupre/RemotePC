// Undefine thoses...
#ifdef _EXP_FUNC 
#undef _EXP_FUNC
#endif
#ifdef _INTERNAL_LINKAGE   
#undef _INTERNAL_LINKAGE
#endif
#ifdef _EXTERNAL_LINKAGE   
#undef _EXTERNAL_LINKAGE
#endif

#define DONT_COMPILE_AS_DLL
//#define COMPILE_MSVC_DLL
//#define COMPILE_FOR_BORLAND
//#ifdef COMPILE_FOR_BORLAND
	//#define COMPILE_DELPHI_DLL
	//#define COMPILE_CPPBLD_DLL
//#endif

// Redefine them...
#ifdef DONT_COMPILE_AS_DLL
	#define _INTERNAL_LINKAGE
	#define _EXTERNAL_LINKAGE
	#define _EXP_FUNC 
#else
	#ifdef COMPILE_MSVC_DLL
		#define _INTERNAL_LINKAGE
		#define _EXTERNAL_LINKAGE
		#define _EXP_FUNC  __declspec(dllexport)
	#else
		#define COMPILE_BORLAND_DLL
		#ifdef COMPILE_DELPHI_DLL
			#define _INTERNAL_LINKAGE __cdecl
			#define _EXTERNAL_LINKAGE __cdecl
			#define _EXP_FUNC         __declspec(dllexport)
		#else
			#define _INTERNAL_LINKAGE __cdecl
			#define _EXTERNAL_LINKAGE __stdcall
			#ifdef _BUILD_DLL_
				#define _EXP_FUNC     __declspec(dllexport)
			#else
				#define _EXP_FUNC     __declspec(dllimport)
			#endif
		#endif
	#endif
#endif
