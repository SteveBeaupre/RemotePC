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
	CLoginInfo LoginInfo;
	CNetManager NetManager;
	
	bool WriteData(BYTE *pBuf, int BufSize);
	void CalcScreenSize(int *w, int *h);
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

	void SetLoginInfo(char *pUserName, char *pPassword);
	void OnLoginRequest(LoginInfoStruct *pInfo);
	void SendLoginResult(bool Succeded);

	void OnMouseMsg();
	void OnKeyboardMsg();
	
	void OnScreenshotRequest();
	void SendScreenshot();
};
