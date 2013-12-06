#include "RemotePCClient.h"

CRemotePCClient::CRemotePCClient()
{

}

CRemotePCClient::~CRemotePCClient()
{

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

	ScreenshotManager.Decompress(pMsgData, pMsgHeader->MsgSize);
	MessageBeep(0);
}

void CRemotePCClient::SendMouseMsg()
{

}

void CRemotePCClient::SendKeyboardMsg()
{

}

