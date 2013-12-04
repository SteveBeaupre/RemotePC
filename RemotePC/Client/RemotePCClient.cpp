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

	const int HeaderSize = sizeof(MsgHeaderStruct);
	const int BufSize = HeaderSize + MsgHeader.MsgSize;

	CRawBuffer Buffer(BufSize);

	int Indx = 0;
	memcpy(Buffer.GetBuffer(Indx), &MsgHeader, HeaderSize); 
	Indx += HeaderSize;
	
	memcpy(Buffer.GetBuffer(Indx), &LoginInfo, MsgHeader.MsgSize);

	WriteData(Buffer.GetBuffer(0), Buffer.GetSize());
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

