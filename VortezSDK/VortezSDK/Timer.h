#ifndef CTIMER_H
#define CTIMER_H

#pragma comment(lib, "winmm.lib")

#include "Preproc.h"

#include <Windows.h>
#include "Conversion.h"

#ifdef COMPILE_FOR_BORLAND
class ATimer {
public:
	virtual void  _INTERNAL_LINKAGE Reset() = 0;
	virtual float _INTERNAL_LINKAGE Tick() = 0;

	virtual void  _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CTimer  : public ATimer {
#else
class _EXP_FUNC CTimer {
#endif
public:
	CTimer();
private:
	bool    IsPerfCounterAvailable;
	float   TimeScale;
	__int64 PerfCounterFrequency;
private:
	__int64 LastTime;
	__int64 CurrentTime;
public:
	void  _INTERNAL_LINKAGE Reset();
	float _INTERNAL_LINKAGE Tick();

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#ifdef COMPILE_FOR_BORLAND
class ATimer64 {
public:
	virtual void   _INTERNAL_LINKAGE Reset() = 0;
	virtual double _INTERNAL_LINKAGE Tick() = 0;

	virtual void  _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CTimer64  : public ATimer64 {
#else
class _EXP_FUNC CTimer64 {
#endif
public:
	CTimer64();
private:
	bool     IsPerfCounterAvailable;
	double   TimeScale;
	__int64  PerfCounterFrequency;
private:
	__int64 LastTime;
	__int64 CurrentTime;
public:
	void   _INTERNAL_LINKAGE Reset();
	double _INTERNAL_LINKAGE Tick();

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#ifdef COMPILE_FOR_BORLAND
class AGameTimer {
public:
	virtual void  _INTERNAL_LINKAGE Reset() = 0;
	virtual float _INTERNAL_LINKAGE Tick() = 0;
	virtual float _INTERNAL_LINKAGE GetFPS() = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CGameTimer  : public AGameTimer {
#else
class _EXP_FUNC CGameTimer {
#endif
public:
	CGameTimer();
private:
	bool    IsPerfCounterAvailable;
	float   TimeScale;
	__int64 PerfCounterFrequency;
private:
	__int64 LastTime;
	__int64 CurrentTime;
private:
	float FPS;
	float FPS_ElapsedTime;
public:
	void  _INTERNAL_LINKAGE Reset();
	float _INTERNAL_LINKAGE Tick();
	float _INTERNAL_LINKAGE GetFPS();

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

/*#ifndef RAD_STUDIO_XE
#ifdef COMPILE_FOR_BORLAND
class ACycleTimer {
public:
	virtual void _INTERNAL_LINKAGE Start() = 0;
	virtual UI64 _INTERNAL_LINKAGE Stop() = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif


#ifdef COMPILE_FOR_BORLAND
class CCycleTimer  : public ACycleTimer {
#else
class _EXP_FUNC CCycleTimer {
#endif
public:
	CCycleTimer();
private:
	UI64 StartCycle;
	UI64 Overhead;
public:
	UI64 GetOverhead();
public:
	void _INTERNAL_LINKAGE Start();
	UI64 _INTERNAL_LINKAGE Stop();

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};
#endif*/

#endif //--CTIMER_H