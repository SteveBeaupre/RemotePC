#include "Crono.h"

//---------------------------------------------------------------------------
// Get the frequency and current time
//---------------------------------------------------------------------------
CCrono::CCrono()
{
	QueryPerformanceFrequency(&Freq);
	Start();
}

//---------------------------------------------------------------------------
// Get the current time
//---------------------------------------------------------------------------
void CCrono::Start()
{
	QueryPerformanceCounter(&StartTicks);
}

//---------------------------------------------------------------------------
// Get the current elapsed time since Start() and returns it in microseconds
//---------------------------------------------------------------------------
DWORD CCrono::Tick(BOOL Reset)
{
	if(!Reset){
		QueryPerformanceCounter(&StopTicks);
		return (DWORD)(1000000*(StopTicks.QuadPart - StartTicks.QuadPart)/Freq.QuadPart);
	} else {
		Start();
		return 0;
	}
}

//---------------------------------------------------------------------------
// Wait the requested delay in microseconds
//---------------------------------------------------------------------------
void CCrono::Wait(DWORD Delay)
{
	Start();
	while(Tick() < Delay);
}
