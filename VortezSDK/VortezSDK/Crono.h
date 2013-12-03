#ifndef CCRONO_H
#define CCRONO_H

#pragma comment(lib, "winmm.lib")
#include <Windows.h>

#include "Preproc.h"

#ifdef COMPILE_FOR_BORLAND
// Create an abstract class of the object to export
class ACrono {
public:
	virtual void  _INTERNAL_LINKAGE Start() = 0;
	virtual DWORD _INTERNAL_LINKAGE Tick(BOOL Reset = false) = 0;
	virtual void  _INTERNAL_LINKAGE Wait(DWORD Delay) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CCrono : public ACrono
#else
class _EXP_FUNC CCrono
#endif
{
public:
	CCrono();
private:
	LARGE_INTEGER StartTicks, StopTicks, Freq;
public:
	void  _INTERNAL_LINKAGE Start();
	DWORD _INTERNAL_LINKAGE Tick(BOOL Reset = false);
	void  _INTERNAL_LINKAGE Wait(DWORD Delay);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif //--CCRONO_H