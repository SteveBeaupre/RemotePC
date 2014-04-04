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
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

DWORD WINAPI WorkerThreadFunc(void* param);

class EXP_FUNC CRemotePC : public IRemotePC {
public:
	CRemotePC();
	~CRemotePC();
protected:
	CLock WriteLock;
	CThread WorkerThread;
	CNetManager NetManager;
protected:	
	void SendMsg(DWORD MsgSize, DWORD MsgID, void *pData);
	void SendMsg(MsgHeaderStruct *pMsgHeader, void *pData);
	bool WriteData(BYTE *pBuf, int BufSize);
public:
	HWND GetHostWnd();
	CNetManager* GetNetManager();
	CThread* GetThread();

	void StartThread();
	void StopThread();
	void CleanupThread();
	
	void ProcessWinsockMessages(LPARAM uMsg);

	bool IsConnected();
	void ConnectAsServer(HWND hWnd, WORD port);
	void ConnectAsClient(HWND hWnd, char *hostname, WORD port);
	void Disconnect();
};
