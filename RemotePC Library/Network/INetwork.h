#pragma once
//----------------------------------------------------------------------//
#define WIN32_LEAN_AND_MEAN 
#define VC_LEANMEAN         
//----------------------------------------------------------------------//
#include "Windows.h"
#include "stdio.h"
//----------------------------------------------------------------------//
#pragma comment(lib, "WSOCK32.lib")  //WinSock lib  
#include "Winsock.h"
//----------------------------------------------------------------------//

enum NetworkMode {
	ServerMode = 0,
	ClientMode,
};

//----------------------------------------------------------------------//

class INetManager {
public:
	virtual NetworkMode GetMode() = 0;
	virtual void SetMode(NetworkMode Mode) = 0;

	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual bool IsConnected() = 0;
	virtual void ConnectAsServer(HWND hWnd, WORD port) = 0;
	virtual void ConnectAsClient(HWND hWnd, char *hostname, WORD port) = 0;
	virtual void Disconnect() = 0;

	virtual SOCKET* GetSocket() = 0;
	virtual HWND GetHostHWND() = 0;

	virtual bool CanRecv() = 0;
	virtual bool CanSend() = 0;

	virtual bool Recv(BYTE *buf, int size, int *indx) = 0;
	virtual bool Send(BYTE *buf, int size, int *indx) = 0;
};

