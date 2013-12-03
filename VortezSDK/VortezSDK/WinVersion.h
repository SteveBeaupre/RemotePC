#ifndef _CWINVERSION_H_
#define _CWINVERSION_H_
#ifdef __cplusplus

#include <Windows.h>

#include "Preproc.h"

#define _WIN_UNKNOW_ -1
#define _WIN_31_     0
#define _WIN_95_     1
#define _WIN_98_     2
#define _WIN_ME_     3
#define _WIN_2000_   4
#define _WIN_XP_     5
#define _WIN_NT_     6
#define _WIN_VISTA_  7
#define _WIN_7_      8

#ifdef COMPILE_FOR_BORLAND
class AWinVersion {
public:
	virtual DWORD _INTERNAL_LINKAGE DetectWindowsVersion();

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif


#ifdef COMPILE_FOR_BORLAND
class CWinVersion : public AWinVersion {
#else
class _EXP_FUNC CWinVersion {
#endif
public:
	DWORD _INTERNAL_LINKAGE DetectWindowsVersion();

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};


#endif
#endif //_CWINVERSION_H_
