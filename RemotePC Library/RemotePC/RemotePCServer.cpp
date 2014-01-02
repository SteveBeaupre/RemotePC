#include "RemotePCServer.h"

CRemotePCServer::CRemotePCServer()
{

}

CRemotePCServer::~CRemotePCServer()
{
	// Just make sure the thread is not running before closing
	if(MultithreadedScreenshot)
		ScreenshotManager.WaitForScreenshotThreadToFinish();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCServer::Reset()
{
	ScreenshotManager.Reset();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCServer::ProcessRemotePCMessages(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData)
{
	switch(pMsgHeader->MsgID)
	{
	case MSG_CLIENT_LOGIN_REQUEST:
		OnLoginRequest((LoginInfoStruct*)pMsgData);
		break;
	case MSG_SCREENSHOT_REQUEST:
		OnScreenshotRequest((ScrFormat*)pMsgData);
		break;
	case MSG_MOUSE_INPUT_DATA:
		OnMouseMsg((CMouseInputMsgStruct*)pMsgData);
		break;
	case MSG_KB_INPUT_DATA:
		OnKeyboardMsg((CKeyboardInputMsgStruct*)pMsgData);
		break;
	}
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCServer::SetLoginInfo(char *pUserName, char *pPassword)
{
	LoginInfo.Reset();
	LoginInfo.SetInfo(pUserName, pPassword);
}

void CRemotePCServer::OnLoginRequest(LoginInfoStruct *pInfo)
{
	GetNetManager()->GetLog()->Log("Login info received\n");

	LoginResults res = LoginInfo.CompareLoginInfo(pInfo);

	SendLoginResult(res);
	
	bool LogedIn = res == NoErrors;
	PostMessage(GetHostWnd(), ON_LOGIN, (BOOL)LogedIn, (UINT)res);

	if(!LogedIn){
		Sleep(1000);
		NetManager.Disconnect();
	}
}

void CRemotePCServer::CalcScreenSize(int *w, int *h)
{
	RECT r;
	HWND hDesktopWnd = GetDesktopWindow();
	GetWindowRect(hDesktopWnd, &r);

	*w = r.right  - r.left;
	*h = r.bottom - r.top;
}

void CRemotePCServer::SendLoginResult(LoginResults Results)
{
	GetNetManager()->GetLog()->Log("Sending Login Info...\n");

	bool Succeded = Results == NoErrors;

	LoginResultStruct LoginResult;
	ZeroMemory(&LoginResult, sizeof(LoginResultStruct));
	LoginResult.Result = Results;
	
	if(Succeded){
		int w,h;
		CalcScreenSize(&w, &h);

		LoginResult.ScrWidth  = w;
		LoginResult.ScrHeight = h;
	}
	
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(LoginResultStruct);
	MsgHeader.MsgID   = Succeded ? MSG_CLIENT_LOGIN_COMPLETED : MSG_CLIENT_LOGIN_FAILED;

	SendMsg(&MsgHeader, &LoginResult);
}

//----------------------------------------------------------------------//

void CRemotePCServer::OnScreenshotRequest(ScrFormat *pFormat)
{
	GetNetManager()->GetLog()->Log("Screenshot Request received\n");

	if(!MultithreadedScreenshot){
		// Set the screenshot format
		ScreenshotManager.SetFormat(*pFormat);

		// Take screenshot normally
		ScreenshotManager.Take();
		CRawBuffer* pBuf = ScreenshotManager.GetCompressedBuffer();
		SendScreenshot(pBuf);
	} else {
		// Wait for the thread to finish
		ScreenshotManager.WaitForScreenshotThreadToFinish();
		
		// Send the buffer
		CRawBuffer* pBuf = ScreenshotManager.GetCompressedBuffer();
		SendScreenshot(pBuf);

		// Set the screenshot format
		ScreenshotManager.SetFormat(*pFormat);

		// Get the next screenshot in advance
		ScreenshotManager.StartScreenshotThread();
	}
}

void CRemotePCServer::StartScreenshotThread()
{
	ScreenshotManager.StartScreenshotThread();
}

void CRemotePCServer::WaitForScreenshotThreadToFinish()
{
	ScreenshotManager.WaitForScreenshotThreadToFinish();
}

void CRemotePCServer::SendScreenshot(CRawBuffer *pBuffer)
{
	GetNetManager()->GetLog()->Log("Sending Screenshot Data...\n");

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = pBuffer->GetSize();
	MsgHeader.MsgID   = MSG_SCREENSHOT_REPLY;

	SendMsg(&MsgHeader, pBuffer->GetBuffer());
}

//----------------------------------------------------------------------//

void CRemotePCServer::OnMouseMsg(CMouseInputMsgStruct* pMsg)
{
	ServerInputs.ProcessMouseInput(pMsg);
}

void CRemotePCServer::OnKeyboardMsg(CKeyboardInputMsgStruct* pMsg)
{
	ServerInputs.ProcessKeyboardInput(pMsg);
}

//----------------------------------------------------------------------//

void CRemotePCServer::SetMultiThreadedMode(bool MultithreadTheScreenshot)
{
	MultithreadedScreenshot = MultithreadTheScreenshot;
}
