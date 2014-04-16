#include "RemotePCClient.h"

CRemotePCClient::CRemotePCClient()
{
	#ifndef EMULATE_OPENGL
	pOpenGL = new COpenGL;
	#else
	pOpenGL = new COpenGLEmulator;
	#endif

	hRendererWnd = NULL;
	ScreenshotFormat = scrf_32;
}

CRemotePCClient::~CRemotePCClient()
{
	pOpenGL->Shutdown();
	SAFE_DELETE_OBJECT(pOpenGL);
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
	/*case MSG_GETDRIVES_REPLY: 
		OnGetDrivesReply((DWORD*)pMsgData);
		break;*/
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

	return pOpenGL->Initialize(hRendererWnd);
}

void CRemotePCClient::ShutdownOpenGL()
{
	pOpenGL->Shutdown();
}

void CRemotePCClient::RenderTexture()
{
	pOpenGL->Render();
}

void CRemotePCClient::ClearScreen()
{
	pOpenGL->RenderEmpty();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::SendLoginRequest(char *pUserName, char *pPassword)
{
	GetNetManager()->GetLog()->Add("Sending Login Request...\n");

	CLoginInfo LoginInfo;

	LoginInfo.SetInfo(pUserName, pPassword);

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(LoginInfoStruct);
	MsgHeader.MsgID   = MSG_CLIENT_LOGIN_REQUEST;

	SendMsg(&MsgHeader, &LoginInfo);
}

void CRemotePCClient::OnLoginResult(LoginResultStruct* pLoginResult)
{
	GetNetManager()->GetLog()->Add("Login result received\n");

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
	GetNetManager()->GetLog()->Add("Sending Screenshot Request...\n");

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(ScreenshotFormat);
	MsgHeader.MsgID   = MSG_SCREENSHOT_REQUEST;

	ScrFormat Format = GetScreenshotFormat();
	SendMsg(&MsgHeader, &Format);
}

void CRemotePCClient::OnScreenshotMsg(MsgHeaderStruct *pMsgHeader, BYTE *pMsgData)
{
	GetNetManager()->GetLog()->Add("Screenshot data received\n");

	DecompressedScreenshotInfoStruct Info;
	ScreenshotManager.Decompress(pMsgData, pMsgHeader->MsgSize, &Info);

	if(Info.pBuffer){
		pOpenGL->LoadTexture(Info.pBuffer->GetBuffer(), Info.Width, Info.Height, Info.Format);
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

//----------------------------------------------------------------------//

/*void CRemotePCClient::SendGetDrivesRequest()
{
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = 0;
	MsgHeader.MsgID   = MSG_GETDRIVES_REQUEST;

	SendMsg(&MsgHeader, NULL);
}

void CRemotePCClient::OnGetDrivesReply(DWORD *pDrives)
{
	SendMessage(GetHostWnd(), WM_AVAILDRIVE_CB_CLEAR, 0, 0);
	
	char c = 0;
	char szDrive[16];
	
	DWORD AvailableDrives = *pDrives;
	for(int i = 0; i < 26; i++){
		if(((AvailableDrives >> i) & 1) > 0){
			c = i + 65;
			ZeroMemory(szDrive, 16);
			sprintf(szDrive, "%c:\\", c);
			SendMessage(GetHostWnd(), WM_AVAILDRIVE_CB_ADDSTR, 0, (LPARAM)(LPCTSTR)szDrive);
		}
	}
}

void CRemotePCClient::SendScanDirectoryRequest()
{
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = 0;
	MsgHeader.MsgID   = MSG_SCAN_DIRECTORY_REQUEST;

	SendMsg(&MsgHeader, NULL);
}

void CRemotePCClient::OnScanDirectoryReply(DWORD *pDrives)
{

}*/

