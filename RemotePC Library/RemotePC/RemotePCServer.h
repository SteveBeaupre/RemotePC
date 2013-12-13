#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "RemotePC.h"
#include "ServerScreenshotManager.h"
//----------------------------------------------------------------------//

class CRemotePCServer : public CRemotePC, public IRemotePCServer {
public:
	CRemotePCServer();
	~CRemotePCServer();
private:
	CLoginInfo LoginInfo;
	CServerScreenshotManager ScreenshotManager;
	CServerInputs ServerInputs;
private:
	void CalcScreenSize(int *w, int *h);
public:
	void Reset();
	void ProcessRemotePCMessages(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData);

	void SetLoginInfo(char *pUserName, char *pPassword);
	void OnLoginRequest(LoginInfoStruct *pInfo);
	void SendLoginResult(bool Succeded);
	
	void StartScreenshotThread();
	void WaitForScreenshotThreadToFinish();

	void OnScreenshotRequest();
	void SendScreenshot(CRawBuffer *pBuffer);

	void OnMouseMsg(CMouseInputMsgStruct* pMsg);
	void OnKeyboardMsg(CKeyboardInputMsgStruct* pMsg);
};
