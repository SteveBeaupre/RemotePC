#ifndef _SOCKETS_LIST_H_
#define _SOCKETS_LIST_H_
#ifdef __cplusplus

#define WIN32_LEAN_AND_MEAN // Trim libraies.
#define VC_LEANMEAN         // Trim even further.
#include "Windows.h"
#include "stdio.h"

#include "Winsock.h"

#include "MsgReader.h"
#include "RawBuffer.h"

#include "Preproc.h"
#include <SafeKill.h>

#define ALL_PLAYERS  0xFFFFFFFF

//----------------------------------------------------------------------//
// 
//----------------------------------------------------------------------//
struct _EXP_FUNC CSocketNode {

	// Class to read the socket network messages
	CMsgReader MsgReader; 
	// A simple socket
	SOCKET Socket;
	
	// Client usefull infos
	CRawBuffer ip;
	UINT Port;

	// Tell if we're logged in or not
	bool IsLogedIn;      
	// The player's name
	UINT PlayerID;    
	// The player's name
	CRawBuffer PlayerName;   

	struct CSocketNode *Prev;
	struct CSocketNode *Next;
};

//----------------------------------------------------------------------//
// 
//----------------------------------------------------------------------//
class _EXP_FUNC CSocketsList {
public:
	CSocketsList();
	~CSocketsList();
private:
	bool TestBit (UINT  bitField, char bitNum){return (bitField & (1<<bitNum)) > 0;}
	void SetBit  (UINT &bitField, char bitNum){bitField |= (1 << bitNum);}
	void ClearBit(UINT &bitField, char bitNum){bitField &= ~(1 << bitNum);}
private:
	UINT NumNodesAllocated;
private:
	UINT AvailableIDs;
	UINT GetFirstAvailableID();
private:
	CSocketNode Root;
	void InitRootNode();
public:
	UINT GetNodeCount();
	UINT GetNumAllocatedNodes(){return NumNodesAllocated;}
private:
	CSocketNode* GetRootNode();
public:
	CSocketNode* GetFirstNode();
	CSocketNode* GetLastNode();
	CSocketNode* GetNode(UINT Indx);
	CSocketNode* GetNodeBySocket(SOCKET s, UINT *pIndx);
public:
	CSocketNode* PushNode(SOCKET s);
	bool PopNode();
	bool DeleteNode(UINT Indx);
	void ClearAll();
};

#endif
#endif //_SOCKETS_LIST_H_
