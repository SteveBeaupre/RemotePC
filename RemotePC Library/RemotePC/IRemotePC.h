#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#ifdef RAD_STUDIO_XE
#pragma comment(lib, "VortezSDK.lib")
#endif
#include "VortezSDK.h"
//----------------------------------------------------------------------//
#include "NetworkManager.h"
#include "Screenshot.h"
#include "Inputs.h"
#include "LoginInfo.h"
#include "MsgHeader.h"
//----------------------------------------------------------------------//

class IRemotePC {
public:
	virtual void Reset() = 0;
	virtual void ProcessRemotePCMessages(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData){}
	
	virtual bool InitOpenGL(){return false;}
	virtual void ShutdownOpenGL(){}
	virtual void RenderTexture(){}
	virtual void ClearScreen(){}

	virtual void WaitForScreenShotThreadToFinish(){}
};

class IRemotePCServer : public IRemotePC {
public:
	virtual void SetLoginInfo(char *pUserName, char *pPassword) = 0;
	virtual void OnLoginRequest(LoginInfoStruct *pInfo) = 0;
	virtual void SendLoginResult(LoginResults Results) = 0;
	
	virtual void OnScreenshotRequest(ScrFormat *pFormat) = 0;
	virtual void SendScreenshot(CRawBuffer *pBuffer) = 0;

	virtual void OnMouseMsg(CMouseInputMsgStruct* pMsg) = 0;
	virtual void OnKeyboardMsg(CKeyboardInputMsgStruct* pMsg) = 0;
};

class IRemotePCClient : public IRemotePC {
public:
	virtual void SendLoginRequest(char *pUserName, char *pPassword) = 0;
	virtual void OnLoginResult(LoginResultStruct* pLoginResult) = 0;
	
	virtual void SendScreenshotRequest() = 0;
	virtual void OnScreenshotMsg(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData) = 0;

	virtual void SendMouseMsg(CMouseInputMsgStruct *mm) = 0;
	virtual void SendKeyboardMsg(CKeyboardInputMsgStruct *km) = 0;
};