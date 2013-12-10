#include "RemotePCClient.h"

CRemotePCClient::CRemotePCClient()
{
	hRendererWnd = NULL;
}

CRemotePCClient::~CRemotePCClient()
{
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

	PostMessage(GetHostWnd(), ON_LOGIN, pLoginResult->LogedIn, 0);
}

void CRemotePCClient::SendScreenshotRequest()
{
	GetNetManager()->GetLog()->Log("Sending Screenshot Request...\n");

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = 0;
	MsgHeader.MsgID   = MSG_SCREENSHOT_REQUEST;

	SendMsg(&MsgHeader, NULL);
}

void CRemotePCClient::OnScreenshotMsg(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData)
{
	GetNetManager()->GetLog()->Log("Screenshot data received\n");

	DecompressedScreenshotInfoStruct Info;
	ScreenshotManager.Decompress(pMsgData, pMsgHeader->MsgSize, &Info);

	if(Info.pBuffer){
		OpenGL.LoadTexture(Info.pBuffer->GetBuffer(), Info.Width, Info.Height, Info.BPP, Info.BPP == 3 ? GL_BGR : GL_BGRA);
		SendScreenshotRequest();
	}	
}

void CRemotePCClient::SendMouseMsg(CMouseInputMsgStruct *mm)
{
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(CMouseInputMsgStruct);
	MsgHeader.MsgID   = MSG_MOUSE_INPUT_DATA;

	SendMsg(&MsgHeader, mm);
}

void CRemotePCClient::SendKeyboardMsg(CKeyboardInputMsgStruct *km)
{

}

