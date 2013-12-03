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

class CRemotePCServer : public IRemotePCServer {
public:
	CRemotePCServer();
	~CRemotePCServer();
private:
	CThread WorkerThread;
	CNetManager NetManager;
public:
	HWND GetHostWnd();

	CThread* GetThread();
	void StartThread();
	void StopThread();
	
	void ProcessWinsockMessages(LPARAM uMsg);

	bool IsConnected();
	void ConnectAsServer(HWND hWnd, WORD port);
	void ConnectAsClient(HWND hWnd, char *hostname, WORD port);
	void Disconnect();

	void OnLoginRequest();
	void SendLoginResult();

	void OnMouseMsg();
	void OnKeyboardMsg();
	
	void OnScreenshotRequest();
	void SendScreenshot();
};
