#pragma once
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#pragma comment(lib, "WSOCK32.lib")  //WinSock lib
#include "Winsock.h"
//----------------------------------------------------------------------//
#include "NetStats.h"
#include "NetLog.h"
#include "NetErrors.h"
#include "WinMsg.h"
//----------------------------------------------------------------------//
#include "Thread.h"
//----------------------------------------------------------------------//
#include "..\\Preproc.h"
//----------------------------------------------------------------------//

#define NO_LOG

bool EXP_FUNC IsWinSockInitialized();
bool EXP_FUNC InitializeWinSock();
void EXP_FUNC ShutdownWinSock();

DWORD WINAPI ConnectToServerThread(void *param);
DWORD WINAPI WaitForClientThread(void *param);

//----------------------------------------------------------------------//
//-------------------CNetBaseEngine Class Definition--------------------//
//----------------------------------------------------------------------//
class EXP_FUNC CNetBase {
protected:
	HWND   m_hWnd;
	SOCKET m_Socket;        
	bool   m_Connected;     

	bool NetIO(DWORD Op, BYTE *buf, int size, int *indx, int MaxPacketSize);
protected:
	CNetLog   Log;
	CNetStats Stats;
public:
	///////////////////////////////////////
	// Mains commands
	///////////////////////////////////////
	void SetHWND(HWND h){m_hWnd = h;};
	HWND GetHWND(){return m_hWnd;};

	SOCKET* GetSocket(){return &m_Socket;}

	void SetConnectionStatus(bool Status){m_Connected = Status;}	
	bool IsConnected(){return m_Connected;}	
	
	CNetLog* GetLog(){return &Log;}
	CNetStats* GetStats(){return &Stats;}

	// Tell us if we can either read or write on the socket
	bool CanRecv();
	bool CanSend();
	// Read/Write data from/to the socket
	bool Recv(BYTE *buf, int size, int *indx);
	bool Send(BYTE *buf, int size, int *indx);
public:
	virtual void Disconnect() = 0;
};


//----------------------------------------------------------------------//
//---------------------CNetServer Class Definition----------------------//
//----------------------------------------------------------------------//
class EXP_FUNC CNetServer : public CNetBase {
public:
	CNetServer();
	~CNetServer();
public:	
	CThread WaitThread;

	void Initialize();

	// Connect to the network
	bool WaitForClient(HWND hHostWnd, short nPort);
	// Disconnect from the Network
	void Disconnect();
};

//----------------------------------------------------------------------//
//---------------------CNetClient Class Definition----------------------//
//----------------------------------------------------------------------//
class EXP_FUNC CNetClient : public CNetBase {
public:
	CNetClient();
	~CNetClient();
private:
	int m_ConTimeOutLen;
	int m_MaxConAttempt;

	SOCKADDR_IN m_saServer; // The server info structure
public:	
	CThread ConnectThread;

	SOCKADDR_IN* GetSockAddrIn(){return &m_saServer;}

	void Initialize();

	int GetConTimeOut(){return m_ConTimeOutLen;}
	int GetConAttempt(){return m_MaxConAttempt;}

	// Connect to the network
	bool ConnectToServer(HWND hHostWnd, char *ip, short nPort, int DefTimeOut, int DefMaxAttempt);
	// Disconnect from the Network
	void Disconnect();
};
