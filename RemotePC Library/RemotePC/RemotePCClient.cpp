#include "RemotePCClient.h"

CRemotePCClient::CRemotePCClient()
{
	hRendererWnd = NULL;
	ScreenshotFormat = scrf_32;
}

CRemotePCClient::~CRemotePCClient()
{
	OpenGL.Shutdown();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::Reset()
{
	ScreenshotManager.Reset();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::ProcessRemotePCMessages(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData)
{
	switch(pMsgHeader->MsgID)
	{
	case MSG_CLIENT_LOGIN_FAILED:    
	case MSG_CLIENT_LOGIN_COMPLETED: 
		OnLoginResult((LoginResultStruct*)pMsgData); 
		break;
	case MSG_SCREENSHOT_REPLY: 
		OnScreenshotMsg(pMsgHeader, pMsgData);
		break;
	}
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::SetRendererWnd(HWND h)
{
	hRendererWnd = h;
}

bool CRemotePCClient::InitOpenGL()
{
	if(!hRendererWnd)
		return false;

	return OpenGL.Initialize(hRendererWnd);
}

void CRemotePCClient::ShutdownOpenGL()
{
	OpenGL.Shutdown();
}

void CRemotePCClient::RenderTexture()
{
	OpenGL.Render();
}

void CRemotePCClient::ClearScreen()
{
	OpenGL.RenderEmpty();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::SendLoginRequest(char *pUserName, char *pPassword)
{
	GetNetManager()->GetLog()->Log("Sending Login Request...\n");

	CLoginInfo LoginInfo;

	LoginInfo.SetInfo(pUserName, pPassword);

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(LoginInfoStruct);
	MsgHeader.MsgID   = MSG_CLIENT_LOGIN_REQUEST;

	SendMsg(&MsgHeader, &LoginInfo);
}

void CRemotePCClient::OnLoginResult(LoginResultStruct* pLoginResult)
{
	GetNetManager()->GetLog()->Log("Login result received\n");

	PostMessage(GetHostWnd(), ON_LOGIN, (BOOL)(pLoginResult->Result == NoErrors), (UINT)pLoginResult->Result);
}

//----------------------------------------------------------------------//

ScrFormat CRemotePCClient::GetScreenshotFormat()
{
	ScrFormat Format;
	FormatLock.Lock();
	Format = ScreenshotFormat;
	FormatLock.Unlock();

	return Format;
}

void CRemotePCClient::SetScreenshotFormat(ScrFormat Format)
{
	FormatLock.Lock();
	ScreenshotFormat = Format;
	FormatLock.Unlock();
}

void CRemotePCClient::SendScreenshotRequest()
{
	GetNetManager()->GetLog()->Log("Sending Screenshot Request...\n");

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(ScreenshotFormat);
	MsgHeader.MsgID   = MSG_SCREENSHOT_REQUEST;

	ScrFormat Format = GetScreenshotFormat();
	SendMsg(&MsgHeader, &Format);
}

void CRemotePCClient::OnScreenshotMsg(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData)
{
	GetNetManager()->GetLog()->Log("Screenshot data received\n");

	DecompressedScreenshotInfoStruct Info;
	ScreenshotManager.Decompress(pMsgData, pMsgHeader->MsgSize, &Info);

	if(Info.pBuffer){
		OpenGL.LoadTexture(Info.pBuffer->GetBuffer(), Info.Width, Info.Height, Info.Format);
		SendScreenshotRequest();
	}	
}

//----------------------------------------------------------------------//

void CRemotePCClient::SendMouseMsg(CMouseInputMsgStruct *mm)
{
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(CMouseInputMsgStruct);
	MsgHeader.MsgID   = MSG_MOUSE_INPUT_DATA;

	SendMsg(&MsgHeader, mm);
}

void CRemotePCClient::SendKeyboardMsg(CKeyboardInputMsgStruct *km)
{
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(CKeyboardInputMsgStruct);
	MsgHeader.MsgID   = MSG_KB_INPUT_DATA;

	SendMsg(&MsgHeader, km);
}

