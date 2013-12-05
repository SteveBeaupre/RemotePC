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

void CRemotePCClient::ProcessRemotePCMessages(MsgHeaderStruct *MsgHeader, BYTE *MsgData)
{
	switch(MsgHeader->MsgID)
	{
	case MSG_CLIENT_LOGIN_FAILED:    PostMessage(GetHostWnd(), ON_LOGIN, FALSE, 0); break;
	case MSG_CLIENT_LOGIN_COMPLETED: PostMessage(GetHostWnd(), ON_LOGIN, TRUE, 0);  break;
	}
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::SendLoginRequest(char *pUserName, char *pPassword)
{
	CLoginInfo LoginInfo;

	LoginInfo.SetInfo(pUserName, pPassword);

	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = sizeof(LoginInfoStruct);
	MsgHeader.MsgID   = MSG_CLIENT_LOGIN_REQUEST;
	
	SendMsg(&LoginInfo, &MsgHeader);
}

void CRemotePCClient::SendMouseMsg()
{

}

void CRemotePCClient::SendKeyboardMsg()
{

}

void CRemotePCClient::OnScreenshotMsg()
{

}

void CRemotePCClient::SendScreenshotRequest()
{

}

