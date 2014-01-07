#include "RemotePC.h"

CRemotePC::CRemotePC()
{

}

CRemotePC::~CRemotePC()
{

}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

bool CRemotePC::IsConnected()
{
	return NetManager.IsConnected();
}

//----------------------------------------------------------------------//

void CRemotePC::ConnectAsServer(HWND hWnd, WORD port)
{
	NetManager.ConnectAsServer(hWnd, port);
}

//----------------------------------------------------------------------//

void CRemotePC::ConnectAsClient(HWND hWnd, char *hostname, WORD port)
{
	NetManager.ConnectAsClient(hWnd, hostname, port);
}

//----------------------------------------------------------------------//

void CRemotePC::Disconnect()
{
	NetManager.Disconnect();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePC::ProcessWinsockMessages(LPARAM uMsg)
{
	if(WSAGETSELECTEVENT((UINT)uMsg) == FD_CLOSE){
		PostMessage(NetManager.GetHostHWND(), ON_CONNECTION_LOST, 0, 0);
		NetManager.Disconnect();
	}
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

HWND CRemotePC::GetHostWnd()
{
	return NetManager.GetHostHWND();
}

//----------------------------------------------------------------------//

CThread* CRemotePC::GetThread()
{
	return &WorkerThread;
}

//----------------------------------------------------------------------//

CNetManager* CRemotePC::GetNetManager()
{
	return &NetManager;	
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePC::StartThread()
{
	WorkerThread.StartThread(WorkerThreadFunc, this);
}

//----------------------------------------------------------------------//

void CRemotePC::StopThread()
{
	WorkerThread.StopThread();
}

//----------------------------------------------------------------------//

void CRemotePC::CleanupThread()
{
	// Cleanup OpenGL (client only)
	ClearScreen();
	ShutdownOpenGL();
	// Wait for screenshot thread to finish (server only)
	WaitForScreenShotThreadToFinish();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

DWORD WINAPI WorkerThreadFunc(void* param)
{
	//BOOL LogedIn = FALSE;

	CRemotePC *pRemotePC = (CRemotePC*)param;
	CThread *pWorkerThread = pRemotePC->GetThread();
	CNetManager *pNetManager = pRemotePC->GetNetManager();

	HWND hWnd = pRemotePC->GetHostWnd();
	PostMessage(hWnd, ON_READLOOP_MSG, READLOOP_START, 0);

	MsgHeaderStruct Header;
	int HeaderSize = sizeof(MsgHeaderStruct);
	int HeaderIndx = 0;
	int DataIndx = 0;
	ZeroMemory(&Header, HeaderSize);

	CRawBuffer Buffer;

	// Initialize the client OpenGL object (all opengl stuff have no effect on server)
	pRemotePC->InitOpenGL();

	try
	{
		while(1) 
		{
			// break the loop when the event is raised
			if(pWorkerThread->MustExitThread())
				break;

			// stop reading the socket if paused
			if(pWorkerThread->IsThreadPaused()){
				pRemotePC->RenderTexture();
				Sleep(25);
				continue;
			}

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
						pRemotePC->ProcessRemotePCMessages(&Header, Buffer.GetBuffer());

						if(Header.MsgID == MSG_SCREENSHOT_REPLY)
							pRemotePC->RenderTexture();

						// Reset the header
						HeaderIndx = 0;
						ZeroMemory(&Header, HeaderSize);
						// Free the buffer
						DataIndx = 0;
						Buffer.Free();
					}					
				
				}			
			}
		} 
	}
	catch (...)
	{
		//MessageBoxA(hWnd, "An Error occured in ReadLoop thread.\nPerforming Cleanup.", "Error!", 0);
		
		pRemotePC->CleanupThread();

		throw;
	} 

	pRemotePC->CleanupThread();

	PostMessage(hWnd, ON_READLOOP_MSG, READLOOP_END, 0);

	return 0;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePC::SendMsg(DWORD MsgSize, DWORD MsgID, void *pData)
{
	MsgHeaderStruct MsgHeader;
	MsgHeader.MsgSize = MsgSize;
	MsgHeader.MsgID   = MsgID;

	SendMsg(&MsgHeader, pData);
}

void CRemotePC::SendMsg(MsgHeaderStruct *pMsgHeader, void *pData)
{
	const int HeaderSize = sizeof(MsgHeaderStruct);
	const int BufSize = HeaderSize + pMsgHeader->MsgSize;

	// Fill the buffer
	CRawBuffer Buffer(BufSize);
	memcpy(Buffer.GetBuffer(0), pMsgHeader, HeaderSize); 
	if(pMsgHeader->MsgSize > 0)
		memcpy(Buffer.GetBuffer(HeaderSize), pData, pMsgHeader->MsgSize);

	WriteData(Buffer.GetBuffer(0), Buffer.GetSize());
}

bool CRemotePC::WriteData(BYTE *pBuf, int BufSize)
{
	int NumBytesWriten = 0;

	WriteLock.Lock();
	while(1)
	{
		if(WorkerThread.MustExitThread()){
			WriteLock.Unlock();
			return false;
		}

		if(NetManager.CanSend()){

			if(!NetManager.Send(pBuf, BufSize, &NumBytesWriten)){
				WriteLock.Unlock();
				return false;
			}

			if(NumBytesWriten == BufSize)
				break;
		}
	}
	WriteLock.Unlock();

	return true;
}
