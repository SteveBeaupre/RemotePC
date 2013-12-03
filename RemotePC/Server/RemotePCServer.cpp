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

DWORD WINAPI WorkerThreadFunc(void* param)
{
	CRemotePCServer *pServer = (CRemotePCServer*)param;
	CThread *pWorkerThread = pServer->GetThread();

	HWND hWnd = pServer->GetHostWnd();
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

void CRemotePCServer::StartThread()
{
	WorkerThread.StartThread(WorkerThreadFunc, this, "RemotePCServerReadLooThreadEvent");
}

void CRemotePCServer::StopThread()
{
	WorkerThread.StopThread();
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//

void CRemotePCServer::OnLoginRequest()
{

}

void CRemotePCServer::SendLoginResult()
{

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

