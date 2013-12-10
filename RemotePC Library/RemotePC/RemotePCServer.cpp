#include "RemotePCServer.h"

CRemotePCServer::CRemotePCServer()
{

}

CRemotePCServer::~CRemotePCServer()
{

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
		OnScreenshotRequest();
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

	bool LogedIn = LoginInfo.CompareLoginInfo(pInfo);
	SendLoginResult(LogedIn);

	PostMessage(GetHostWnd(), ON_LOGIN, (BOOL)LogedIn, 0);

	if(!LogedIn)
		NetManager.Disconnect();
}

void CRemotePCServer::CalcScreenSize(int *w, int *h)
{
	RECT r;
	HWND hDesktopWnd = GetDesktopWindow();
	GetWindowRect(hDesktopWnd, &r);

	*w = r.right  - r.left;
	*h = r.bottom - r.top;
}

void CRemotePCServer::SendLoginResult(bool Succeded)
{
	GetNetManager()->GetLog()->Log("Sending Login Info...\n");

	LoginResultStruct LoginResult;
	ZeroMemory(&LoginResult, sizeof(LoginResultStruct));
	LoginResult.LogedIn = Succeded != false;
	
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

void CRemotePCServer::OnScreenshotRequest()
{
	GetNetManager()->GetLog()->Log("Screenshot Request received\n");

	CRawBuffer* pBuf = ScreenshotManager.Take(TRUE);
	SendScreenshot(pBuf);
}

void CRemotePCServer::SendScreenshot(CRawBuffer *pBuffer)
{
	GetNetManager()->GetLog()->Log("Sending Screenshot Data...\n");

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = pBuffer->GetSize();
	MsgHeader.MsgID   = MSG_SCREENSHOT_REPLY;

	SendMsg(&MsgHeader, pBuffer->GetBuffer());
}

void CRemotePCServer::OnMouseMsg(CMouseInputMsgStruct* pMsg)
{
	ServerInputs.ProcessMouseInput(pMsg);
}

void CRemotePCServer::OnKeyboardMsg(CKeyboardInputMsgStruct* pMsg)
{
	ServerInputs.ProcessKeyboardInput(pMsg);
}

