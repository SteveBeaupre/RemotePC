#ifndef _CTHREAD_H_
#define _CTHREAD_H_
#ifdef __cplusplus

#include "Preproc.h"

#include <Windows.h>
#include <stdio.h>

#define ABORT_IF_RUNNING  0 // Dont start another thread if one is already running
#define WAIT_IF_RUNNING   1 // Wait for the other thread to finish before staring a new one
#define WAIT_FOR_FINISH   2 // Start a new thread and block the calling thread until finish (this one cause the thread handles to be overwriten, rendering further call effective only on the last started thread)

#ifdef COMPILE_FOR_BORLAND
class AThread {
public:
	virtual HANDLE _INTERNAL_LINKAGE GetThreadHandle() = 0;

	virtual bool _INTERNAL_LINKAGE StartThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, char *szEventName, UINT WaitMode = ABORT_IF_RUNNING) = 0;
	virtual void _INTERNAL_LINKAGE StopThread() = 0;      // Trigger the abort signal, see MustExitThread() below

	virtual bool _INTERNAL_LINKAGE IsThreadRunning() = 0; // Tell if the thread is running
	virtual void _INTERNAL_LINKAGE WaitForThread() = 0;   // Wait for the thread to finish, blocking the current one (call before closing the program)
	
	virtual bool _INTERNAL_LINKAGE MustExitThread() = 0;  // Call this inside your thread function (periodically, in a loop) to know if the thread should return and terminate now
};
#endif

//----------------------------------------------------------------------//
//-----------------------CThread Class Definition-----------------------//
//----------------------------------------------------------------------//

#ifdef COMPILE_FOR_BORLAND
class CThread : public AThread {
#else
class _EXP_FUNC CThread { 
#endif
public:
	CThread(); // No destructor == make sure to call WaitForThread() before closing 
	~CThread();
private:
	DWORD  dwThreadID;
	HANDLE hThread;
	HANDLE hThreadEvent;
	void Init();
public:
	HANDLE _INTERNAL_LINKAGE GetThreadHandle(){return hThread;}
/*private:
	CRITICAL_SECTION cs;
public:
	void Lock();
	void Unlock();*/
public:
	bool _INTERNAL_LINKAGE StartThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, char *szEventName, UINT WaitMode = ABORT_IF_RUNNING);
	void _INTERNAL_LINKAGE StopThread();      // Trigger the abort signal, see MustExitThread() below

	bool _INTERNAL_LINKAGE IsThreadRunning(); // Tell if the thread is running
	void _INTERNAL_LINKAGE WaitForThread();   // Wait for the thread to finish, blocking the current one (call before closing the program)
	
	bool _INTERNAL_LINKAGE MustExitThread();  // Call this inside your thread function (periodically, in a loop) to know if the thread should return and terminate now
};

#endif
#endif //_CTHREAD_H_
