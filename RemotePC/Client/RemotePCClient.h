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

class CRemotePCClient : public IRemotePCClient {
public:
	CRemotePCClient();
	~CRemotePCClient();
private:
	CThread WorkerThread;
	CNetManager NetManager;
	
	bool WriteData(BYTE *pBuf, int BufSize);
public:
	HWND GetHostWnd();
	CNetManager* GetNetManager();
	CThread* GetThread();

	void StartThread();
	void StopThread();
	
	void ProcessWinsockMessages(LPARAM uMsg);
	void ProcessRemotePCMessages(MsgHeaderStruct *MsgHeader, BYTE *MsgData);

	bool IsConnected();
	void ConnectAsServer(HWND hWnd, WORD port);
	void ConnectAsClient(HWND hWnd, char *hostname, WORD port);
	void Disconnect();

	void SendLoginRequest(char *pUserName, char *pPassword);

	void SendMouseMsg();
	void SendKeyboardMsg();
	
	void OnScreenshotMsg();
	void SendScreenshotRequest();
};
