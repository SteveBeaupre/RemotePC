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
//----------------------------------------------------------------------//

void CRemotePCClient::ProcessWinsockMessages(LPARAM uMsg)
{
	if(WSAGETSELECTEVENT((UINT)uMsg) == FD_CLOSE){
		PostMessage(NetManager.GetHostHWND(), ON_CONNECTION_LOST, 0, 0);
		NetManager.Disconnect();
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

DWORD WINAPI WorkerThreadFunc(void* param)
{
	CRemotePCClient *pClient = (CRemotePCClient*)param;
	CThread *pWorkerThread = pClient->GetThread();

	HWND hWnd = pClient->GetHostWnd();
	PostMessage(hWnd, ON_READLOOP_MSG, READLOOP_START, 0);

	try
	{
		while(1) 
		{
		
			// break the loop when the event is raised
			if(pWorkerThread->MustExitThread())
				break;

			


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
	WorkerThread.StartThread(WorkerThreadFunc, this, "RemotePCClientReadLooThreadEvent");
}

void CRemotePCClient::StopThread()
{
	WorkerThread.StopThread();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCClient::SendLoginRequest()
{

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

