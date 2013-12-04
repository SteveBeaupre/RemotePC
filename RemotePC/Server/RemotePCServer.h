#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "RemotePC.h"
//----------------------------------------------------------------------//

class CRemotePCServer : public CRemotePC, public IRemotePCServer {
public:
	CRemotePCServer();
	~CRemotePCServer();
private:
	CLoginInfo LoginInfo;
	void CalcScreenSize(int *w, int *h);
public:
	void ProcessRemotePCMessages(MsgHeaderStruct *MsgHeader, BYTE *MsgData);

	void SetLoginInfo(char *pUserName, char *pPassword);
	void OnLoginRequest(LoginInfoStruct *pInfo);
	void SendLoginResult(bool Succeded);

	void OnMouseMsg();
	void OnKeyboardMsg();
	
	void OnScreenshotRequest();
	void SendScreenshot();
};
