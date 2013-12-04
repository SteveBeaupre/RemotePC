#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "IRemotePC.h"
//----------------------------------------------------------------------//

DWORD WINAPI WorkerThreadFunc(void* param);

class CRemotePC : public IRemotePC {
public:
	CRemotePC();
	~CRemotePC();
protected:
	CThread WorkerThread;
	CNetManager NetManager;
protected:	
	bool WriteData(BYTE *pBuf, int BufSize);
public:
	HWND GetHostWnd();
	CNetManager* GetNetManager();
	CThread* GetThread();

	void StartThread();
	void StopThread();
	
	void ProcessWinsockMessages(LPARAM uMsg);

	bool IsConnected();
	void ConnectAsServer(HWND hWnd, WORD port);
	void ConnectAsClient(HWND hWnd, char *hostname, WORD port);
	void Disconnect();
};
