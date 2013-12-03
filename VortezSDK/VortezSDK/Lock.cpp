#include "Lock.h"

CLock::CLock()
{
	InitializeCriticalSection(&cs);
}

CLock::~CLock()
{
	DeleteCriticalSection(&cs);
}

void CLock::Lock()
{
	EnterCriticalSection(&cs);
}

void CLock::Unlock()
{
	LeaveCriticalSection(&cs);
}


