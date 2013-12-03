#ifndef _MSG_READER_H_
#define _MSG_READER_H_
#ifdef __cplusplus

#define WIN32_LEAN_AND_MEAN // Trim libraies.
#define VC_LEANMEAN         // Trim even further.

#include "Windows.h"
#include "Winsock.h"

#include "Packet.h"

#include <SafeKill.h>

#define ON_WINSOCK_MESSAGE    WM_USER + 120
#define ON_NETWORK_MESSAGE    WM_USER + 121

#define MAX_PLAYER_NAME_LEN   32
#define MAX_LOGIN_PASS_LEN    32
#define MAX_IP_LEN            16 
#define MAX_VERNUM_LEN         8

struct _EXP_FUNC CNetworkMsg {
	DWORD MsgSize;
	DWORD MsgID;
	BYTE* pData;
};

struct _EXP_FUNC CLoginRequestMsg {
	DWORD MsgSize;
	DWORD MsgID;
	
	char VerNum[MAX_VERNUM_LEN];
	char LoginPass[MAX_LOGIN_PASS_LEN];
	char PlayerName[MAX_PLAYER_NAME_LEN];
};

struct _EXP_FUNC CLoginReplyMsg {
	DWORD MsgSize;
	DWORD MsgID;
	
	DWORD Res;
};

//----------------------------------------------------------------------//
// The class used to buffer data input/output
//----------------------------------------------------------------------//
class _EXP_FUNC CMsgReader {
public:
	CMsgReader();
private:
	CReadResult Read(SOCKET *pSock, UINT Source, DWORD BufSize, __int64 *pByteRecv);
public:
	CPacket MsgBuffer;

	void Reset();
	BOOL ReadMsg(SOCKET *pSock, HWND hHostWnd, __int64 *pByteRecv);
};


#endif
#endif //_MSG_READER_H_
