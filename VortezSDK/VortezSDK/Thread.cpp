#include "Thread.h"

CThread::CThread()
{
	//InitializeCriticalSection(&cs);
	Init();
}

CThread::~CThread()
{
	//DeleteCriticalSection(&cs);
}

void CThread::Init()
{
	dwThreadID   = 0;
	hThread      = NULL;
	hThreadEvent = NULL;
}

void CThread::WaitForThread()
{
	WaitForSingleObject(hThread, INFINITE);
}

bool CThread::StartThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, char *szEventName, UINT WaitMode)
{
	// Wait on the other thread to finish if asked to
	if(WaitMode == WAIT_IF_RUNNING)
		WaitForThread();

	// Make sure the thread isn't already running...
	if(!IsThreadRunning()){
		if(hThreadEvent)
			CloseHandle(hThreadEvent);
		// Create event to stop the read thread and writing loop
		hThreadEvent = CreateEvent(NULL, TRUE, FALSE, szEventName);
		// Lauch the thread
		hThread = CreateThread(NULL, 0, lpStartAddress, lpParameter, 0, &dwThreadID);	

		// Wait on the thread to finish if asked to
		if(WaitMode == WAIT_FOR_FINISH)
			WaitForThread();

		return true;
	}
	
	return false;
}

void CThread::StopThread()
{
	// This will wait for the read thread to terminate, if running
	SetEvent(hThreadEvent);
	WaitForThread();
	ResetEvent(hThreadEvent);
	
	// Call CloseHandle() on our handles
	CloseHandle(hThread);
	CloseHandle(hThreadEvent);

	// Reinitialize our handles
	Init();
}

bool CThread::MustExitThread()
{
	// Return true if the even is set
	return WaitForSingleObject(hThreadEvent, 0) == WAIT_OBJECT_0;
}

bool CThread::IsThreadRunning()
{
	// Return true if the thread is running
	return WaitForSingleObject(hThread, 0) == WAIT_TIMEOUT;
}

/*void CThread::Lock()
{
	EnterCriticalSection(&cs);
}

void CThread::Unlock()
{
	LeaveCriticalSection(&cs);
}*/

