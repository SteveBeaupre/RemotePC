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
	virtual HWND GetHostWnd() = 0;
	virtual CNetManager* GetNetManager() = 0;
	virtual CThread* GetThread() = 0;

	virtual void StartThread() = 0;
	virtual void StopThread()  = 0;
	
	virtual void ProcessWinsockMessages(LPARAM uMsg) = 0;
	virtual void ProcessRemotePCMessages(MsgHeaderStruct *MsgHeader, BYTE *MsgData) = 0;

	virtual bool IsConnected() = 0;
	virtual void ConnectAsServer(HWND hWnd, WORD port) = 0;
	virtual void ConnectAsClient(HWND hWnd, char *hostname, WORD port) = 0;
	virtual void Disconnect() = 0;
};

class IRemotePCServer : public IRemotePC {
public:
	virtual void SetLoginInfo(char *pUserName, char *pPassword) = 0;
	virtual void OnLoginRequest(LoginInfoStruct *pInfo) = 0;
	virtual void SendLoginResult(bool Succeded) = 0;

	virtual void OnMouseMsg() = 0;
	virtual void OnKeyboardMsg() = 0;
	
	virtual void OnScreenshotRequest() = 0;
	virtual void SendScreenshot() = 0;
};

class IRemotePCClient : public IRemotePC {
public:
	virtual void SendLoginRequest(char *pUserName, char *pPassword) = 0;

	virtual void SendMouseMsg() = 0;
	virtual void SendKeyboardMsg() = 0;
	
	virtual void OnScreenshotMsg() = 0;
	virtual void SendScreenshotRequest() = 0;
};