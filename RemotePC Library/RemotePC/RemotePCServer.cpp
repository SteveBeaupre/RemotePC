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

void CRemotePCServer::ProcessRemotePCMessages(MsgHeaderStruct *MsgHeader, BYTE *MsgData)
{
	switch(MsgHeader->MsgID)
	{
	case MSG_CLIENT_LOGIN_REQUEST:
		OnLoginRequest((LoginInfoStruct*)MsgData);
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

	SendMsg(&LoginResult, &MsgHeader);
}

void CRemotePCServer::OnMouseMsg()
{

}

void CRemotePCServer::OnKeyboardMsg()
{

}

void CRemotePCServer::OnScreenshotRequest()
{

}

void CRemotePCServer::SendScreenshot()
{

}

