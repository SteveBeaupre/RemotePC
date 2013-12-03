#ifndef _CVORTEZ_NETWORK_ENGINE_H_
#define _CVORTEZ_NETWORK_ENGINE_H_
#ifdef __cplusplus
//----------------------------------------------------------------------//
#pragma comment(lib, "WSOCK32.lib")
//#pragma comment(lib, "VortezSDK.lib")
//----------------------------------------------------------------------//
#include <VortezSDK.h>
#include "Preproc.h"
//----------------------------------------------------------------------//
#include <Windows.h>
#include <stdio.h>
#include <Winsock.h>
//----------------------------------------------------------------------//
#include "SocketsList.h"
//----------------------------------------------------------------------//
#include <SafeKill.h>
//----------------------------------------------------------------------//
#define MSG_LOGIN_REQUEST        0x00010000
#define MSG_LOGIN_DATA           0x00020000
#define MSG_LOGIN_REPLY          0x00030000

#define MSG_ON_PLAYER_LOGED_IN   0x00040000
#define MSG_ON_PLAYER_LOGED_OFF  0x00050000
#define MSG_EVERYONE_READY       0x00060000

#define MSG_ON_CHAT_MSG          0x00070000

#define ERR_SERVER_FULL              1
#define ERR_INVALID_SOCKET           2
#define ERR_SOCKET_ALREADY_IN_USE    3

#define ERR_LOGIN_PASS_INVALID       1
#define ERR_PLAYER_NAME_IN_USE       2
#define ERR_VERSION_DONT_MATCH       3

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
#ifdef COMPILE_CPPBLD_DLL
#ifdef __cplusplus
extern "C" {
#endif
#endif
_EXP_FUNC bool _EXTERNAL_LINKAGE InitializeWinsock();
_EXP_FUNC void _EXTERNAL_LINKAGE ShutdownWinsock();
#ifdef COMPILE_CPPBLD_DLL
#ifdef __cplusplus
}
#endif
#endif

#ifdef COMPILE_FOR_BORLAND
class AVortezNetworkServer {
public:
	virtual bool _INTERNAL_LINKAGE StartServer(HWND hWnd, short nPort, char *pLoginPass, UINT MaxPlayers, char *pVerNum) = 0;
	virtual void _INTERNAL_LINKAGE DisconnectClient(SOCKET s = 0);
	virtual void _INTERNAL_LINKAGE DisconnectClients() = 0;
	virtual void _INTERNAL_LINKAGE Disconnect() = 0;
	virtual bool _INTERNAL_LINKAGE IsConnected() = 0;

	virtual bool _INTERNAL_LINKAGE SendMsg(SOCKET s, CNetworkMsg *pMsg) = 0;
	virtual bool _INTERNAL_LINKAGE SendChatMsg(SOCKET s, UINT PlayerID, char* pPlayerName, char *pTextData) = 0;

	virtual UINT _INTERNAL_LINKAGE GetMaxPlayersValue() = 0;
	virtual UINT _INTERNAL_LINKAGE GetNumPlayersConnected() = 0;

	virtual HWND    _INTERNAL_LINKAGE GetHostWnd() = 0;
	virtual SOCKET  _INTERNAL_LINKAGE GetSocket() = 0;
	virtual SOCKET* _INTERNAL_LINKAGE GetSocketPtr() = 0;

	virtual void _INTERNAL_LINKAGE ProcessWinsockMsg(HWND hWnd, WPARAM wParam, LPARAM lParam) = 0;
	virtual void _INTERNAL_LINKAGE ProcessNetworkMsg(HWND hWnd, WPARAM wParam, LPARAM lParam) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

//----------------------------------------------------------------------//
//-------------CVortezNetworkServer Class Definition--------------------//
//----------------------------------------------------------------------//
#ifdef COMPILE_FOR_BORLAND
class CVortezNetworkServer : public AVortezNetworkServer 
#else
class _EXP_FUNC CVortezNetworkServer  
#endif
{
public:
	CVortezNetworkServer();
	~CVortezNetworkServer();
private:
	HWND   m_HostWnd;
	SOCKET m_Socket;        
	UINT   m_MaxPlayers;
	char   m_VerNum[MAX_VERNUM_LEN];
	char   m_LoginPass[MAX_LOGIN_PASS_LEN];
private:
	__int64 Downloaded, Uploaded;
public:
	__int64 GetNumBytesUploaded(){return Uploaded;}
	__int64 GetNumBytesDownloaded(){return Downloaded;}
public:
	bool BypassLoginSystem;

	bool AllowDuplicatedUserName;
	bool AllowMismatchVersionNumber;
	bool AllowInvalidPassword;
public:
	CSocketsList SocketList;   
private:
	bool SendLoginRequestMsg(SOCKET s);
	bool SendLoginReplyMsg(SOCKET s, WORD PlayerID, WORD Err);
	
	bool SendPlayerLogedInMsg(SOCKET s, char *pPlayerName);
	bool SendPlayerLogedOffMsg(SOCKET s, char *pPlayerName);
	bool SendEveryoneReadyMsg(SOCKET s);
public:
	bool _INTERNAL_LINKAGE Listen(HWND hWnd, WORD nPort, UINT MaxPlayers, char *pLoginPass, char *pVerNum);
	void _INTERNAL_LINKAGE DisconnectClient(SOCKET s);
	void _INTERNAL_LINKAGE DisconnectClients();
	void _INTERNAL_LINKAGE Disconnect();
	bool _INTERNAL_LINKAGE IsConnected();
public:
	bool _INTERNAL_LINKAGE SendMsg(SOCKET s, CNetworkMsg *pMsg);
	bool _INTERNAL_LINKAGE SendChatMsg(SOCKET s, UINT PlayerID, char* pPlayerName, char *pTextData);
public:
	UINT _INTERNAL_LINKAGE GetMaxPlayersValue(){return m_MaxPlayers;}
	UINT _INTERNAL_LINKAGE GetNumPlayersConnected(){return SocketList.GetNodeCount();}
public:
	HWND    _INTERNAL_LINKAGE GetHostWnd(){return m_HostWnd;}
	SOCKET  _INTERNAL_LINKAGE GetSocket(){return m_Socket;}
	SOCKET* _INTERNAL_LINKAGE GetSocketPtr(){return &m_Socket;}
public:
	void _INTERNAL_LINKAGE ProcessWinsockMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void _INTERNAL_LINKAGE ProcessNetworkMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
public:
#ifndef COMPILE_FOR_BORLAND
	virtual void OnConnectionAccepted(CSocketNode *pNode){}
	virtual void OnConnectionRefused(UINT Reason){}
	virtual void OnConnectionClosed(CSocketNode *pNode){}

	virtual void OnClientLogedIn(CSocketNode *pNode){}
	virtual void OnClientLoginFailed(CSocketNode *pNode, UINT Err){}
	//virtual void OnClientLoginTimedOut (CSocketNode *pNode){}

	virtual void OnMaxPlayersLoged(){}

	virtual void OnChatMsg(UINT PlayerID, char *pPlayerName, char *pTextData){}
	virtual void OnCustomNetworkMsgRecv(CSocketNode *pNode, CNetworkMsg *pMsg){}	

	virtual void OnServerListening(SOCKET s){}	
	virtual void OnServerDisconnected(SOCKET s){}	
	virtual void OnClientDisconnected(CSocketNode *pNode){}	
#endif
public:
#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

class CVortezNetworkServerExt : public CVortezNetworkServer {
public:
	void OnConnectionAccepted(CSocketNode *pNode);
	void OnConnectionRefused(UINT Reason);
	void OnConnectionClosed(CSocketNode *pNode);
	void OnClientLogedIn(CSocketNode *pNode);
	void OnClientLoginFailed(CSocketNode *pNode, UINT Err);
	void OnMaxPlayersLoged();
	void OnChatMsg(UINT PlayerID, char *pPlayerName, char *pTextData);
	void OnCustomNetworkMsgRecv(CSocketNode *pNode, CNetworkMsg *pMsg);
	void OnServerListening(SOCKET s);
	void OnServerDisconnected(SOCKET s);
	void OnClientDisconnected(CSocketNode *pNode);
};

#ifdef COMPILE_FOR_BORLAND
class AVortezNetworkClient {
public:
	virtual bool _INTERNAL_LINKAGE ConnectToServer(HWND hWnd, char *ip, short nPort, char *pPlayerName, char *pLoginPass, char *pVerNum, UINT uConnTimeOutLen = 1000, UINT uNumConnAttempts = 15) = 0;
	virtual void _INTERNAL_LINKAGE Disconnect() = 0;
	virtual bool _INTERNAL_LINKAGE IsConnected() = 0;

	virtual HWND    _INTERNAL_LINKAGE GetHostWnd() = 0;
	virtual SOCKET  _INTERNAL_LINKAGE GetSocket() = 0;
	virtual SOCKET* _INTERNAL_LINKAGE GetSocketPtr() = 0;
	virtual SOCKADDR_IN  _INTERNAL_LINKAGE GetSockAddrIn() = 0;
	virtual SOCKADDR_IN* _INTERNAL_LINKAGE GetSockAddrInPtr() = 0;

	virtual bool _INTERNAL_LINKAGE SendMsg(CNetworkMsg *pMsg) = 0;
	virtual bool _INTERNAL_LINKAGE SendChatMsg(UINT PlayerID, char* pPlayerName, char *pTextData) = 0;

	virtual void _INTERNAL_LINKAGE ProcessWinsockMsg(HWND hWnd, WPARAM wParam, LPARAM lParam) = 0;
	virtual void _INTERNAL_LINKAGE ProcessNetworkMsg(HWND hWnd, WPARAM wParam, LPARAM lParam) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

//----------------------------------------------------------------------//
//-------------CVortezNetworkClient Class Definition--------------------//
//----------------------------------------------------------------------//
class _EXP_FUNC CVortezNetworkClient { 
public:
	CVortezNetworkClient();
	~CVortezNetworkClient();
private:
	HWND m_HostWnd;

	SOCKET      m_Socket;
	SOCKADDR_IN saServer;

	CMsgReader  MsgReader;

	CRawBuffer  m_PlayerName; 
	CRawBuffer  m_LoginPass; 
	CRawBuffer  m_VerNum; 
private:
	__int64 Downloaded, Uploaded;
public:
	__int64 GetNumBytesUploaded(){return Uploaded;}
	__int64 GetNumBytesDownloaded(){return Downloaded;}
private:
	bool SendLoginDataMsg();
public:
	CThread Thread;
public:
	UINT ConnTimeOutLen;
	UINT NumConnAttempts;
public:
	bool _INTERNAL_LINKAGE ConnectToServer(HWND hWnd, char *ip, WORD nPort, char *pPlayerName, char *pLoginPass, char *pVerNum, UINT uConnTimeOutLen = 1000, UINT uNumConnAttempts = 15);
	void _INTERNAL_LINKAGE Disconnect();
	bool _INTERNAL_LINKAGE IsConnected();
public:
	HWND    _INTERNAL_LINKAGE GetHostWnd(){return m_HostWnd;}
	SOCKET  _INTERNAL_LINKAGE GetSocket(){return m_Socket;}
	SOCKET* _INTERNAL_LINKAGE GetSocketPtr(){return &m_Socket;}
	SOCKADDR_IN  _INTERNAL_LINKAGE GetSockAddrIn(){return saServer;}
	SOCKADDR_IN* _INTERNAL_LINKAGE GetSockAddrInPtr(){return &saServer;}
public:
	bool _INTERNAL_LINKAGE SendMsg(CNetworkMsg *pMsg);
	bool _INTERNAL_LINKAGE SendChatMsg(UINT PlayerID, char* pPlayerName, char *pTextData);
public:
	void _INTERNAL_LINKAGE ProcessWinsockMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void _INTERNAL_LINKAGE ProcessNetworkMsg(HWND hWnd, WPARAM wParam, LPARAM lParam);
public:
#ifndef COMPILE_FOR_BORLAND
	virtual void OnConnectionThreadStarted(char *ip, WORD Port){}
	virtual void OnConnectionEstablished(SOCKET s){}
	virtual void OnConnectionLoss(SOCKET s, UINT ErrCode){}
	virtual void OnConnectionFailed(UINT AttemptNumber, UINT NumAttempts){}
	virtual void OnConnectionTimedOut(){}
	virtual void OnConnectionCanceled(){}
	
	virtual void OnLoginSuccess(SOCKET s, UINT PlayerID){}
	virtual void OnLoginFailed(SOCKET s, UINT Err){}

	virtual void OnEveryoneReady(){}
	virtual void OnPlayerLogedIn(char *pPlayerName){}
	virtual void OnPlayerLogedOff(char *pPlayerName){}

	virtual void OnChatMsg(UINT PlayerID, char *pPlayerName, char *pTextData){}
	virtual void OnCustomNetworkMsgRecv(SOCKET s, CNetworkMsg *pMsg){}	

	virtual void OnDisconnect(SOCKET s){}	
#endif
public:
#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

class _EXP_FUNC CVortezNetworkClientExt : public CVortezNetworkClient {
public:
	void OnConnectionThreadStarted(char *ip, WORD Port);
	void OnConnectionEstablished(SOCKET s);
	void OnConnectionLoss(SOCKET s, UINT ErrCode);
	void OnConnectionFailed(UINT AttemptNumber, UINT NumAttempts);
	void OnConnectionTimedOut();
	void OnConnectionCanceled();
	void OnLoginSuccess(SOCKET s, UINT PlayerID);
	void OnLoginFailed(SOCKET s, UINT Err);
	void OnEveryoneReady();
	void OnPlayerLogedIn(char *pPlayerName);
	void OnPlayerLogedOff(char *pPlayerName);
	void OnChatMsg(UINT PlayerID, char *pPlayerName, char *pTextData);
	void OnCustomNetworkMsgRecv(SOCKET s, CNetworkMsg *pMsg);
	void OnDisconnect(SOCKET s);
};

//----------------------------------------------------------------------//

#endif
#endif //_CVORTEZ_NETWORK_ENGINE_H_
