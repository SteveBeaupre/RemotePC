#include "SocketsList.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CSocketsList::CSocketsList()
{
	NumNodesAllocated = 0;
	AvailableIDs = ALL_PLAYERS;
	InitRootNode();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CSocketsList::~CSocketsList()
{
	ClearAll();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UINT CSocketsList::GetFirstAvailableID()
{
	// Loop through all the available id until one is available
	for(int Cpt = 0; Cpt <= 31; Cpt++){
		if(TestBit(AvailableIDs, Cpt)){
			ClearBit(AvailableIDs, Cpt); 
			return Cpt+1;
		}	
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CSocketsList::InitRootNode()
{
	Root.Socket = INVALID_SOCKET;
	Root.IsLogedIn = false;
	Root.PlayerID = 0;

	Root.Prev = NULL;
	Root.Next = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UINT CSocketsList::GetNodeCount()
{
	UINT Count = 0;
	CSocketNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL){
		pNode = pNode->Next;
		Count++;
	}

	// Return the number of nodes of this pile
	return Count;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CSocketNode* CSocketsList::GetNode(UINT Indx)
{
	UINT Count = 0;
	CSocketNode *pNode = &Root;
	
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

CSocketNode* CSocketsList::GetRootNode()
{
	return &Root;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CSocketNode* CSocketsList::GetFirstNode()
{
	CSocketNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	if(pNode->Next != NULL)
		return pNode->Next;
	else
		return NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////////

CSocketNode* CSocketsList::GetLastNode()
{
	CSocketNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL)
		pNode = pNode->Next;

	// Return the last node pointer
	return pNode;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CSocketNode* CSocketsList::GetNodeBySocket(SOCKET s, UINT *pIndx)
{
	*pIndx = -1;
	CSocketNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the one we want
	while(pNode->Next != NULL){
		(*pIndx)++;
		pNode = pNode->Next;
		if(pNode->Socket == s)
			return pNode;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CSocketNode* CSocketsList::PushNode(SOCKET s)
{
	CSocketNode *pNode = GetLastNode();

	// Allocate the new node
	pNode->Next = new CSocketNode;
	NumNodesAllocated++;

	CSocketNode *pNewNode = pNode->Next;
	pNewNode->Socket = s;
	pNewNode->IsLogedIn = false;
	pNewNode->PlayerID = GetFirstAvailableID();
	pNewNode->PlayerName.Allocate(MAX_PLAYER_NAME_LEN);
	pNewNode->ip.Allocate(16);
	pNewNode->Port = 0;


	// Update pointers
	pNewNode->Next = NULL;
	pNewNode->Prev = pNode;

	return pNewNode;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CSocketsList::PopNode()
{
	CSocketNode *pNode = GetLastNode();

	// If this the root node, we're done...
	if(pNode == NULL || pNode == &Root){return false;} // return faillure

	// Make the player id available again...
	SetBit(AvailableIDs, pNode->PlayerID - 1);

	// Free the player's name and ip buffer
	pNode->PlayerName.Free();
	pNode->ip.Free();

	// Update pointers
	pNode->Prev->Next = NULL;

	// Delete the node
	SAFE_DELETE_OBJECT(pNode);
	NumNodesAllocated--;

	// return success
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CSocketsList::DeleteNode(UINT Indx)
{
	CSocketNode *pNode = GetNode(Indx);

	// If this the root node, we're done...
	if(!pNode || pNode == &Root){return false;} // return faillure

	// Make the player id available again...
	SetBit(AvailableIDs, pNode->PlayerID - 1);

	// Free the player's name and ip buffer
	pNode->PlayerName.Free();
	pNode->ip.Free();

	// Update pointers
	CSocketNode *pPrevNode = pNode->Prev;
	CSocketNode *pNextNode = pNode->Next;

	pPrevNode->Next  = pNextNode;
	if(pNextNode)
		pNextNode->Prev = pPrevNode;

	// Delete the node
	SAFE_DELETE_OBJECT(pNode);
	NumNodesAllocated--;

	// return success
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CSocketsList::ClearAll()
{
	// Delete everything...
	while(PopNode());
	// Just to make sure...
	AvailableIDs = ALL_PLAYERS;
}
