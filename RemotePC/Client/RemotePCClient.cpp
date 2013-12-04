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

HWND CRemotePCClient::GetHostWnd()
{
	return NetManager.GetHostHWND();
}

//----------------------------------------------------------------------//

CThread* CRemotePCClient::GetThread()
{
	return &WorkerThread;
}

//----------------------------------------------------------------------//

CNetManager* CRemotePCClient::GetNetManager()
{
	return &NetManager;	
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::ProcessWinsockMessages(LPARAM uMsg)
{
	if(WSAGETSELECTEVENT((UINT)uMsg) == FD_CLOSE){
		PostMessage(NetManager.GetHostHWND(), ON_CONNECTION_LOST, 0, 0);
		NetManager.Disconnect();
	}
}

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

bool CRemotePCClient::IsConnected()
{
	return NetManager.IsConnected();
}

void CRemotePCClient::ConnectAsServer(HWND hWnd, WORD port)
{
	NetManager.ConnectAsServer(hWnd, port);
}

void CRemotePCClient::ConnectAsClient(HWND hWnd, char *hostname, WORD port)
{
	NetManager.ConnectAsClient(hWnd, hostname, port);
}

void CRemotePCClient::Disconnect()
{
	NetManager.Disconnect();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

DWORD WINAPI ClientWorkerThreadFunc(void* param)
{
	//BOOL LogedIn = FALSE;

	CRemotePCClient *pClient = (CRemotePCClient*)param;
	CThread *pWorkerThread = pClient->GetThread();
	CNetManager *pNetManager = pClient->GetNetManager();

	HWND hWnd = pClient->GetHostWnd();
	PostMessage(hWnd, ON_READLOOP_MSG, READLOOP_START, 0);

	MsgHeaderStruct Header;
	int HeaderSize = sizeof(MsgHeaderStruct);
	int HeaderIndx = 0;
	int DataIndx = 0;
	ZeroMemory(&Header, HeaderSize);

	CRawBuffer Buffer;

	try
	{
		while(1) 
		{
			// break the loop when the event is raised
			if(pWorkerThread->MustExitThread())
				break;

			// Socket ready to read?
			if(pNetManager->CanRecv()){
			
				// Reading msg header?
				if(HeaderIndx < HeaderSize){
					
					// Read it...
					if(!pNetManager->Recv((BYTE*)&Header, HeaderSize, &HeaderIndx))
						break;

					// Are we done reading the header?
					if(HeaderIndx == HeaderSize){
						
						// Is the message size zero?
						if(Header.MsgSize == 0){
							// Process the message below...
							goto ProcessNetMsgJmp;
						} else {
							// Allocate a buffer to receive the msg data
							Buffer.Allocate(Header.MsgSize);
						}							
					}					
				
				} else { // Reading the msg data...
					
					// Read it...
					if(!pNetManager->Recv((BYTE*)Buffer.GetBuffer(), Buffer.GetSize(), &DataIndx))
						break;

					// Are we done reading the data?
					if(DataIndx == (int)Buffer.GetSize()){
						// Jump from above
						ProcessNetMsgJmp:

						//
						pClient->ProcessRemotePCMessages(&Header, Buffer.GetBuffer());

						// Reset the header
						HeaderIndx = 0;
						ZeroMemory(&Header, HeaderSize);
						// Free the buffer
						Buffer.Free();
					}					
				
				}			
			}

			// Not to fast :)
			Sleep(100);
		
		} // while
	}
	catch (...)
	{
		MessageBoxA(hWnd, "An Error occured in ReadLoop thread.\nPerforming Cleanup.", "Error!", 0);
		//CleanupReadLoopThread(hWnd, &VideoEncoder, &OriginalDesktopColor);
		throw;
	} 

	PostMessage(hWnd, ON_READLOOP_MSG, READLOOP_END, 0);

	return 0;
}

void CRemotePCClient::StartThread()
{
	WorkerThread.StartThread(ClientWorkerThreadFunc, this, "RemotePCClientReadLooThreadEvent");
}

void CRemotePCClient::StopThread()
{
	WorkerThread.StopThread();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

bool CRemotePCClient::WriteData(BYTE *pBuf, int BufSize)
{
	int NumBytesWriten = 0;
	while(1) // put program in an infinite loop of writing data
	{
		if(NetManager.CanSend()){
			
			if(!NetManager.Send(pBuf, BufSize, &NumBytesWriten))
				return false;

			if(NumBytesWriten == BufSize)
				break; 
		} else {
			return false;
		}
	}

	return true;
}

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

