#ifndef _CNETENGINE_H_
#define _CNETENGINE_H_
#ifdef __cplusplus
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
//----------------------------------------------------------------------//
#pragma comment(lib, "WSOCK32.lib")  //WinSock lib
#include "Winsock.h"
//----------------------------------------------------------------------//
#include "NetLog.h"
#include "NetErrors.h"
#include "WinMsg.h"
//----------------------------------------------------------------------//
#include "Thread.h"
//----------------------------------------------------------------------//

#define NO_LOG

bool IsWinSockInitialized();
bool InitializeWinSock();
void ShutdownWinSock();

DWORD WINAPI ConnectToServerThread(void *param);
DWORD WINAPI WaitForClientThread(void *param);

//----------------------------------------------------------------------//
//-------------------CNetBaseEngine Class Definition--------------------//
//----------------------------------------------------------------------//
class CNetBase { 
protected:
	HWND   m_hWnd;
	SOCKET m_Socket;        
	bool   m_Connected;     

	bool NetIO(DWORD Op, BYTE *buf, int size, int *indx, int MaxPacketSize);
protected:
	CNetLog Log;
protected:
	CRITICAL_SECTION BPSCritSec;
	void InitBPSCritSect();
	void DelBPSCritSect();

	__int64 TotalBytesRecved, TotalBytesSended;
	void ResetBytesCounters();
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

	__int64 GetBytesRecved();
	__int64 GetBytesSended();
	void GetBytesExchanged(__int64 *pRecved, __int64 *pSended);

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
class CNetServer : public CNetBase {
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
class CNetClient : public CNetBase {
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


//----------------------------------------------------------------------//

#endif
#endif //_CNETENGINE_H_
