#include "BuffersList.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CBuffersList::CBuffersList()
{
	NumNodesAllocated = 0;
	InitRootNode();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CBuffersList::~CBuffersList()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CBuffersList::InitRootNode()
{
	// To customize...
	Root.pBuffer = NULL;
	Root.BufferSize = 0;

	Root.Prev = NULL;
	Root.Next = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UINT CBuffersList::GetNodesCount()
{
	UINT Count = 0;
	CBuffersListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL){
		pNode = pNode->Next;
		Count++;
	}

	// Return the number of nodes of this pile
	return Count;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UINT CBuffersList::GetTotalBuffersSizes()
{
	UINT res = 0;
	CBuffersListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL){
		pNode = pNode->Next;
		res += pNode->BufferSize;
	}

	// Return the number of nodes of this pile
	return res;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CBuffersList::ExtractBuffers(BYTE *pOut)
{
	CBuffersListNode *pNode = &Root;

	UINT BufIndx = 0;
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL){
		pNode = pNode->Next;
		
		if(pNode->BufferSize > 0){
			memcpy(&pOut[BufIndx], pNode->pBuffer, pNode->BufferSize);
			BufIndx += pNode->BufferSize;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CBuffersListNode* CBuffersList::GetNode(UINT Indx)
{
	UINT Count = 0;
	CBuffersListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the one we want
	while(pNode->Next != NULL){
		pNode = pNode->Next;
		if(Count == Indx)
			return pNode;
		Count++;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CBuffersListNode* CBuffersList::GetRootNode()
{
	return &Root;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CBuffersListNode* CBuffersList::GetFirstNode(bool CanReturnRoot)
{
	CBuffersListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	if(pNode->Next != NULL)
		pNode = pNode->Next;

	if(CanReturnRoot)
		return pNode;
	
	// Return the last node pointer
	return pNode == &Root ? NULL : pNode;
}


/////////////////////////////////////////////////////////////////////////////////////////////////

CBuffersListNode* CBuffersList::GetLastNode(bool CanReturnRoot)
{
	CBuffersListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL)
		pNode = pNode->Next;

	if(CanReturnRoot)
		return pNode;

	// Return the last node pointer
	return pNode == &Root ? NULL : pNode;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CBuffersList::Push(void *pBuffer, UINT BufferSize)
{
	if(BufferSize == 0)
		return;

	CBuffersListNode *pNode = GetLastNode(true);

	// Allocate the new node
	pNode->Next = new CBuffersListNode;
	NumNodesAllocated++;
	CBuffersListNode *pNewNode = pNode->Next;

	// Update pointers
	pNewNode->Next = NULL;
	pNewNode->Prev = pNode;

	// To customize...
	pNewNode->pBuffer = new BYTE[BufferSize];
	memcpy(pNewNode->pBuffer, pBuffer, BufferSize);
	pNewNode->BufferSize = BufferSize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CBuffersList::Pop()
{
	CBuffersListNode *pNode = GetLastNode(true);

	// If this the root node, we're done...
	if(pNode == NULL || pNode == &Root){return false;} // return faillure

	// Update pointers
	pNode->Prev->Next = NULL;

	// Delete the node's data
	SAFE_DELETE_ARRAY(pNode->pBuffer);
	pNode->BufferSize = 0;

	// Delete the node
	SAFE_DELETE_OBJECT(pNode);
	NumNodesAllocated--;

	// return success
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CBuffersList::Clear()
{
	// Delete everything...
	while(Pop());
}

