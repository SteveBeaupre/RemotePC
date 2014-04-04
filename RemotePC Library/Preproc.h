#ifdef COMPILE_AS_LIB
#define EXP_FUNC __declspec(dllexport)
#else
#define EXP_FUNC 
#endif
