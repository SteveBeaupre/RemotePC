#ifndef _BUFFERS_LIST_H_
#define _BUFFERS_LIST_H_
#ifdef __cplusplus

#define WIN32_LEAN_AND_MEAN // Trim libraies.
#define VC_LEANMEAN         // Trim even further.
#include "Windows.h"
#include "stdio.h"
#include "SafeKill.h"

//----------------------------------------------------------------------//
// 
//----------------------------------------------------------------------//
#ifndef ROOT_NODE
	#define ROOT_NODE    -1
#endif

//----------------------------------------------------------------------//
// 
//----------------------------------------------------------------------//
struct CBuffersListNode {
	// To customize...
	BYTE *pBuffer;
	UINT BufferSize;

	struct CBuffersListNode *Prev;
	struct CBuffersListNode *Next;
};

//----------------------------------------------------------------------//
// 
//----------------------------------------------------------------------//
class CBuffersList {
public:
	CBuffersList();
	~CBuffersList();
private:
	UINT NumNodesAllocated;
private:
	CBuffersListNode Root;
	void  InitRootNode();
private:
	CBuffersListNode* GetRootNode();
public:
	CBuffersListNode* GetNode(UINT Indx);
	CBuffersListNode* GetFirstNode(bool CanReturnRoot = false);
	CBuffersListNode* GetLastNode(bool CanReturnRoot = false);
	
	UINT GetNodesCount();
	UINT GetNumAllocatedNodes(){return NumNodesAllocated;}
	UINT GetTotalBuffersSizes();
	void ExtractBuffers(BYTE *pOut);
public:
	void Push(void *pBuffer, UINT BufferSize);
	bool Pop();
	void Clear();
};

#endif
#endif //_BUFFERS_LIST_H_
