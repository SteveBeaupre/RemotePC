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
#include "Thread.h"
//----------------------------------------------------------------------//

//////////////////////////////////////////////
#define MAX_PACKET_SIZE				4096
//////////////////////////////////////////////
#define WM_SOCKET					WM_USER + 150
//////////////////////////////////////////////
// Shared
#define ON_THREAD_START				WM_USER + 151
#define ON_CONNECTED				WM_USER + 152
#define ON_DISCONNECTED				WM_USER + 153
#define ON_CONNECTION_LOST			WM_USER + 154
//////////////////////////////////////////////
// Server only
#define ON_CONNECTION_CANCELED		WM_USER + 155
//////////////////////////////////////////////
// Client only
#define ON_CONNECTION_TIMED_OUT		WM_USER + 156
#define ON_LOGIN					WM_USER + 157
//////////////////////////////////////////////
#define ON_READLOOP_MSG				WM_USER + 158
//////////////////////////////////////////////
#define READLOOP_START				1
#define READLOOP_END				2
#define READLOOP_SS_HEADER_SEND		3
#define READLOOP_SS_HEADER_RECV		3
#define READLOOP_SS_DATA_SEND		4
#define READLOOP_SS_DATA_RECV		4
#define READLOOP_KB_DATA_SEND		5
#define READLOOP_KB_DATA_RECV		5
#define READLOOP_MS_DATA_SEND		6
#define READLOOP_MS_DATA_RECV		6
//////////////////////////////////////////////
//#define ON_PROGRESS_MSG          WM_USER + 159
#define ON_PASSWORD_MSG          WM_USER + 160
//////////////////////////////////////////////
#define ON_CLOSE_FULLSCR_WND_MSG WM_USER + 161
#define ON_SHOW_SCROLLSBARS_MSG  WM_USER + 162
//////////////////////////////////////////////
//////////////////////////////////////////////
#define NET_IO_READ     1
#define NET_IO_WRITE    2
//////////////////////////////////////////////
#define NET_IO_BUFFER_LIMIT_REACH  0x00010000
#define NET_IO_PACKET_LIMIT_REACH  0x00020000
//////////////////////////////////////////////
#define BPS_STACK_SIZE  100
//////////////////////////////////////////////

//----------------------------------------------------------------------//

/*struct CIOResults {
	DWORD Res;
	bool  IsPacketLimitReach;
	bool  IsBufferLimitReach;
};*/

//----------------------------------------------------------------------//

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
	SOCKET m_Socket;        // The connection Socket
	bool   m_Connected;     // Tell if we are connected or not

	bool NetIO(DWORD Op, BYTE *buf, int size, int *indx, int MaxPacketSize);
protected:
	CRITICAL_SECTION BPSCritSec;
	void InitBPSCritSect();
	void DelBPSCritSect();

	__int64 TotalBytesRecved, TotalBytesSended;
	void ResetBytesCounters();
public:
	
	HWND HostHWND;          // The handle of the main client windows
	
	///////////////////////////////////////
	// Mains commands
	///////////////////////////////////////

	__int64 GetBytesRecved();
	__int64 GetBytesSended();
	void GetBytesExchanged(__int64 *pRecved, __int64 *pSended);

	// Tell us if we can either read or write on the socket
	bool CanRecv();
	bool CanSend();

	// Read/Write data from/to the socket
	bool Recv(BYTE *buf, int size, int *indx);
	bool Send(BYTE *buf, int size, int *indx);
	
	// Return the "this" pointer of this class object
	void* GetClassPtr(){return (void*)this;}  // Return the "this" pointer value
	
	///////////////////////////////////////
	// Get/Set commands
	///////////////////////////////////////

	SOCKET* GetSocket(){return &m_Socket;}

	bool Connected(){return m_Connected;}	
	void SetConnectionStatus(bool Status){m_Connected = Status;}	

	bool IsConnected(){return m_Connected;}	
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
