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
#define EMULATE_OPENGL

#ifndef FAKE_OPENGL
#ifndef EMULATE_OPENGL
#include "OpenGL.h"
#endif
#endif

#ifdef FAKE_OPENGL
#include "FakeOpenGL.h"
#endif

#ifdef EMULATE_OPENGL
#include "OpenGLEmulator.h"
#endif
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

class EXP_FUNC CRemotePCClient : public CRemotePC, public IRemotePCClient {
public:
	CRemotePCClient();
	~CRemotePCClient();
private:
	ScrFormat ScreenshotFormat;
	CLock FormatLock;
private:
	IOpenGL* pOpenGL;
private:
	HWND hRendererWnd;
	CClientScreenshotManager ScreenshotManager;
	CClientInputs ClientInputs;
public:
	IOpenGL* GetOpenGL(){return pOpenGL;}
	CClientScreenshotManager* GetScreenshotManager(){return &ScreenshotManager;}
	CClientInputs* GetClientInputs(){return &ClientInputs;}

	void Reset();
	void ProcessRemotePCMessages(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData);

	void SetRendererWnd(HWND h);
	bool InitOpenGL();
	void ShutdownOpenGL();
	void RenderTexture();
	void ClearScreen();

	void SendLoginRequest(char *pUserName, char *pPassword);
	void OnLoginResult(LoginResultStruct* pLoginResult);

	ScrFormat GetScreenshotFormat();
	void SetScreenshotFormat(ScrFormat Format);

	void SendScreenshotRequest();
	void OnScreenshotMsg(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData);

	void SendMouseMsg(CMouseInputMsgStruct *mm);
	void SendKeyboardMsg(CKeyboardInputMsgStruct *km);
};
