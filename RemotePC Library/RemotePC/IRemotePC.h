#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#pragma comment(lib, "VortezSDK.lib")
#include "VortezSDK.h"
//----------------------------------------------------------------------//
#include "NetworkManager.h"
#include "LoginInfo.h"
#include "RawBuffer.h"
#include "MsgHeader.h"
//----------------------------------------------------------------------//

class IRemotePC {
public:
	virtual void Reset() = 0;
	virtual void ProcessRemotePCMessages(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData){}
	
	virtual bool InitOpenGL(){return false;}
	virtual void ShutdownOpenGL(){}
	virtual void RenderTexture(){}
};

class IRemotePCServer : public IRemotePC {
public:
	virtual void SetLoginInfo(char *pUserName, char *pPassword) = 0;
	virtual void OnLoginRequest(LoginInfoStruct *pInfo) = 0;
	virtual void SendLoginResult(bool Succeded) = 0;
	
	virtual void OnScreenshotRequest() = 0;
	virtual void SendScreenshot(CRawBuffer *pBuffer) = 0;

	virtual void OnMouseMsg() = 0;
	virtual void OnKeyboardMsg() = 0;
};

class IRemotePCClient : public IRemotePC {
public:
	virtual void SendLoginRequest(char *pUserName, char *pPassword) = 0;
	virtual void OnLoginResult(LoginResultStruct* pLoginResult) = 0;
	
	virtual void SendScreenshotRequest() = 0;
	virtual void OnScreenshotMsg(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData) = 0;

	virtual void SendMouseMsg() = 0;
	virtual void SendKeyboardMsg() = 0;
};