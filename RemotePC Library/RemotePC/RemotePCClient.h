#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "RemotePC.h"
#include "ClientScreenshotManager.h"
//----------------------------------------------------------------------//

class CRemotePCClient : public CRemotePC, public IRemotePCClient {
public:
	CRemotePCClient();
	~CRemotePCClient();
private:
	CClientScreenshotManager ScreenshotManager;
public:
	void ProcessRemotePCMessages(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData);

	void SendLoginRequest(char *pUserName, char *pPassword);
	void OnLoginResult(LoginResultStruct* pLoginResult);
	
	void SendScreenshotRequest();
	void OnScreenshotMsg(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData);

	void SendMouseMsg();
	void SendKeyboardMsg();
};
