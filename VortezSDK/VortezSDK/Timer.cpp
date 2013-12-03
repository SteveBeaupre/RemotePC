#include "Timer.h"

CTimer::CTimer()
{
	//Check if a High resolution timer is available 
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&PerfCounterFrequency)){ 
		IsPerfCounterAvailable = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime); 
		TimeScale = 1.0f / PerfCounterFrequency;
	} else { 
		IsPerfCounterAvailable = false;
		CurrentTime = timeGetTime(); 
		TimeScale	= 0.001f;
    } 
	
	Reset();
}

void CTimer::Reset()
{
	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Initialization
	LastTime = CurrentTime;
}

float CTimer::Tick()
{
	LastTime = CurrentTime;

	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Calculate the elapsed time
	float ElapsedTime = (CurrentTime - LastTime) * TimeScale;

	return ElapsedTime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

CTimer64::CTimer64()
{
	//Check if a High resolution timer is available 
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&PerfCounterFrequency)){ 
		IsPerfCounterAvailable = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime); 
		TimeScale = 1.0 / PerfCounterFrequency;
	} else { 
		IsPerfCounterAvailable = false;
		CurrentTime = timeGetTime(); 
		TimeScale	= 0.001;
    } 
	
	Reset();
}

void CTimer64::Reset()
{
	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Initialization
	LastTime = CurrentTime;
}

double CTimer64::Tick()
{
	LastTime = CurrentTime;

	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Calculate the elapsed time
	double ElapsedTime = (CurrentTime - LastTime) * TimeScale;

	return ElapsedTime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGameTimer::CGameTimer()
{
	//Check if a High resolution timer is available 
	if(QueryPerformanceFrequency((LARGE_INTEGER*)&PerfCounterFrequency)){ 
		IsPerfCounterAvailable = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime); 
		TimeScale = 1.0f / PerfCounterFrequency;
	} else { 
		IsPerfCounterAvailable = false;
		CurrentTime = timeGetTime(); 
		TimeScale	= 0.001f;
    } 
	
	Reset();
}

void CGameTimer::Reset()
{
	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Initialization
	LastTime = CurrentTime;
	
	FPS = 0;
	FPS_ElapsedTime = 0.0f;
}

float CGameTimer::Tick()
{
	LastTime = CurrentTime;

	if(IsPerfCounterAvailable){
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	} else {
		CurrentTime = timeGetTime();
	}

	// Calculate the elapsed time
	float ElapsedTime = (CurrentTime - LastTime) * TimeScale;

	// Calculate the elapsed time for our fps
	static int Frame = 0;
	Frame++;
	FPS_ElapsedTime += ElapsedTime;
	if(FPS_ElapsedTime >= 1.0f){
		FPS = (float)Frame / FPS_ElapsedTime;
		Frame = 0;
		while(FPS_ElapsedTime >= 1.0f)
			FPS_ElapsedTime -= 1.0f;
	}

	return ElapsedTime;
}

float CGameTimer::GetFPS()
{
	return FPS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*#ifndef RAD_STUDIO_XE
inline UI64 GetCycleCount()
{
  _asm _emit 0x0F
  _asm _emit 0x31
}

CCycleTimer::CCycleTimer()
{
	Overhead = 0;
	Start();
	Overhead = Stop();
}

void CCycleTimer::Start()
{
	StartCycle = GetCycleCount();
}

UI64 CCycleTimer::Stop()
{
	return GetCycleCount() - (StartCycle - Overhead);
}

UI64 CCycleTimer::GetOverhead()
{
	return Overhead;
}
#endif*/

