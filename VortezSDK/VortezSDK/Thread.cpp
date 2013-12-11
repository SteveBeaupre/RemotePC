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
	dwThreadID = 0;
	hThread    = NULL;
	hStopThreadEvent  = NULL;
	hPauseThreadEvent = NULL;
}

void CThread::WaitForThread()
{
	WaitForSingleObject(hThread, INFINITE);
}

bool CThread::StartThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, UINT WaitMode)
{
	// Wait on the other thread to finish if asked to
	if(WaitMode == WAIT_IF_RUNNING)
		WaitForThread();

	// Make sure the thread isn't already running...
	if(!IsThreadRunning()){
		if(hStopThreadEvent)
			CloseHandle(hStopThreadEvent);
		
		// Create event to stop the read thread and writing loop
		hStopThreadEvent  = CreateEvent(NULL, TRUE, FALSE, NULL);
		hPauseThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
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
	ResetEvent(hPauseThreadEvent);
	SetEvent(hStopThreadEvent);
	WaitForThread();
	ResetEvent(hStopThreadEvent);
	
	// Call CloseHandle() on our handles
	CloseHandle(hThread);
	CloseHandle(hStopThreadEvent);
	CloseHandle(hPauseThreadEvent);

	// Reinitialize our handles
	Init();
}

bool CThread::MustExitThread()
{
	// Return true if the even is set
	return WaitForSingleObject(hStopThreadEvent, 0) == WAIT_OBJECT_0;
}

bool CThread::IsThreadRunning()
{
	// Return true if the thread is running
	return WaitForSingleObject(hThread, 0) == WAIT_TIMEOUT;
}

void CThread::PauseThread()
{
	if(IsThreadRunning())
		SetEvent(hPauseThreadEvent);
}

void CThread::ResumeThread()
{
	if(IsThreadRunning())
		ResetEvent(hPauseThreadEvent);
}

bool CThread::IsThreadPaused()
{
	if(!IsThreadRunning())
		return false;

	// Return true if the even is set
	return WaitForSingleObject(hPauseThreadEvent, 0) == WAIT_OBJECT_0;
}

/*void CThread::Lock()
{
	EnterCriticalSection(&cs);
}

void CThread::Unlock()
{
	LeaveCriticalSection(&cs);
}*/

