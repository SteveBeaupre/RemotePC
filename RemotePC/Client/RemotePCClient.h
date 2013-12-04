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

class CRemotePCClient : public CRemotePC, public IRemotePCClient {
public:
	CRemotePCClient();
	~CRemotePCClient();
public:
	void ProcessRemotePCMessages(MsgHeaderStruct *MsgHeader, BYTE *MsgData);

	void SendLoginRequest(char *pUserName, char *pPassword);

	void SendMouseMsg();
	void SendKeyboardMsg();
	
	void OnScreenshotMsg();
	void SendScreenshotRequest();
};
