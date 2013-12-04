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
	// FIX ME LATER !! (should be encapsulated in an object)
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
		// FIX ME LATER !!
		int w,h;
		CalcScreenSize(&w, &h);

		LoginResult.ScrWidth  = w;
		LoginResult.ScrHeight = h;
	}
	
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(LoginResultStruct);
	MsgHeader.MsgID   = Succeded ? MSG_CLIENT_LOGIN_COMPLETED : MSG_CLIENT_LOGIN_FAILED;

	const int HeaderSize = sizeof(MsgHeaderStruct);
	const int BufSize = HeaderSize + MsgHeader.MsgSize;

	CRawBuffer Buffer(BufSize);

	int Indx = 0;
	memcpy(Buffer.GetBuffer(Indx), &MsgHeader, HeaderSize); 
	Indx += HeaderSize;
	
	memcpy(Buffer.GetBuffer(Indx), &LoginResult, MsgHeader.MsgSize);

	WriteData(Buffer.GetBuffer(0), Buffer.GetSize());
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

