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

HWND CRemotePCServer::GetHostWnd()
{
	return NetManager.GetHostHWND();
}

//----------------------------------------------------------------------//

CNetManager* CRemotePCServer::GetNetManager()
{
	return &NetManager;	
}

//----------------------------------------------------------------------//

CThread* CRemotePCServer::GetThread()
{
	return &WorkerThread;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCServer::ProcessWinsockMessages(LPARAM uMsg)
{
	if(WSAGETSELECTEVENT((UINT)uMsg) == FD_CLOSE){
		PostMessage(NetManager.GetHostHWND(), ON_CONNECTION_LOST, 0, 0);
		NetManager.Disconnect();
	}
}

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

bool CRemotePCServer::IsConnected()
{
	return NetManager.IsConnected();
}

void CRemotePCServer::ConnectAsServer(HWND hWnd, WORD port)
{
	NetManager.ConnectAsServer(hWnd, port);
}

void CRemotePCServer::ConnectAsClient(HWND hWnd, char *hostname, WORD port)
{
	NetManager.ConnectAsClient(hWnd, hostname, port);
}

void CRemotePCServer::Disconnect()
{
	NetManager.Disconnect();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

bool CRemotePCServer::WriteData(BYTE *pBuf, int BufSize)
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


DWORD WINAPI ServerWorkerThreadFunc(void* param)
{
	//BOOL LogedIn = FALSE;

	CRemotePCServer *pServer = (CRemotePCServer*)param;
	CThread *pWorkerThread = pServer->GetThread();
	CNetManager *pNetManager = pServer->GetNetManager();

	HWND hWnd = pServer->GetHostWnd();
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
						pServer->ProcessRemotePCMessages(&Header, Buffer.GetBuffer());

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

void CRemotePCServer::StartThread()
{
	WorkerThread.StartThread(ServerWorkerThreadFunc, this, "RemotePCServerReadLooThreadEvent");
}

void CRemotePCServer::StopThread()
{
	WorkerThread.StopThread();
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

