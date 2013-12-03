#include "NetworkManager.h"

CNetManager::CNetManager()
{
	SetMode(ServerMode);
	Initialize();
}

CNetManager::~CNetManager()
{
	Shutdown();
}

NetworkMode CNetManager::GetMode()
{
	return Mode;
}

void CNetManager::SetMode(NetworkMode m)
{
	Mode = m;
	switch(Mode)
	{
	case ServerMode: pNetwork = (CNetBase*)&Server; break;
	case ClientMode: pNetwork = (CNetBase*)&Client; break;
	}
}

void CNetManager::Initialize()
{
	Server.Initialize();
	Client.Initialize();
}

void CNetManager::Shutdown()
{
	Client.Disconnect();
	Server.Disconnect();
}

void CNetManager::ConnectAsServer(HWND hWnd, WORD port)
{
	SetMode(ServerMode);
	Server.WaitForClient(hWnd, port);
}

void CNetManager::ConnectAsClient(HWND hWnd, char *hostname, WORD port)
{
	SetMode(ClientMode);
	Client.ConnectToServer(hWnd, hostname, port, 5000, 10);
}

void CNetManager::Disconnect()
{
	switch(Mode)
	{
	case ServerMode: Server.Disconnect(); break;
	case ClientMode: Client.Disconnect(); break;
	}
}

SOCKET* CNetManager::GetSocket()
{
	return pNetwork->GetSocket();
}

bool CNetManager::IsConnected()
{
	return pNetwork->IsConnected();
}

HWND CNetManager::GetHostHWND()
{
	return pNetwork->HostHWND;
}

bool CNetManager::CanRecv()
{
	return pNetwork->CanRecv();
}

bool CNetManager::CanSend()
{
	return pNetwork->CanSend();
}

bool CNetManager::Recv(BYTE *buf, int size, int *indx)
{
	return pNetwork->Recv(buf, size, indx);
}

bool CNetManager::Send(BYTE *buf, int size, int *indx)
{
	return pNetwork->Send(buf, size, indx);
}

