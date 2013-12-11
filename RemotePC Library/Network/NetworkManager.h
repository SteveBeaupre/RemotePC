#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#include "INetwork.h"
#include "NetEngine.h"
//----------------------------------------------------------------------//

class CNetManager : public INetManager {
public:
	CNetManager();
	~CNetManager();
private:
	NetworkMode Mode;
	CNetServer  Server;
	CNetClient  Client;
	CNetBase    *pNetwork;
public:
	NetworkMode GetMode();
	void SetMode(NetworkMode m);

	void Initialize();
	void Shutdown();

	bool IsConnected();
	void ConnectAsServer(HWND hWnd, WORD port);
	void ConnectAsClient(HWND hWnd, char *hostname, WORD port);
	void Disconnect();

	HWND GetHostHWND();
	SOCKET* GetSocket();
	CNetLog* GetLog();
	CNetStats* GetStats();

	bool CanRecv();
	bool CanSend();
	bool Recv(BYTE *buf, int size, int *indx);
	bool Send(BYTE *buf, int size, int *indx);
};
