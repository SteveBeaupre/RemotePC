#include "StringsList.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

CStringsList::CStringsList()
{
	NumNodesAllocated = 0;
	InitRootNode();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CStringsList::~CStringsList()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CStringsList::InitRootNode()
{
	Root.Indx = (UINT)(-1);

	// To customize...
	Root.s = "";

	Root.Prev = NULL;
	Root.Next = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CStringsList::RecreateIndex()
{
	UINT Count = 0;
	CStringsListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL){
		pNode = pNode->Next;
		pNode->Indx = Count;
		Count++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Strings* CStringsList::GetStringPointer(UINT Indx)
{
	CStringsListNode *pNode = GetNode(Indx);
	return pNode != NULL ? &pNode->s : NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UINT CStringsList::GetNodesCount()
{
	/*UINT Count = 0;
	CStringsListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL){
		pNode = pNode->Next;
		Count++;
	}

	// Return the number of nodes of this pile
	return Count;*/
	return NumNodesAllocated;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CStringsList::CStringsListNode* CStringsList::GetNode(UINT Indx)
{
	UINT Count = 0;
	CStringsListNode *pNode = &Root;
	
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

CStringsList::CStringsListNode* CStringsList::GetRootNode()
{
	return &Root;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CStringsList::CStringsListNode* CStringsList::GetFirstNode(bool CanReturnRoot)
{
	CStringsListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	if(pNode->Next != NULL)
		pNode = pNode->Next;

	if(CanReturnRoot)
		return pNode;
	
	// Return the last node pointer
	return pNode == &Root ? NULL : pNode;
}


/////////////////////////////////////////////////////////////////////////////////////////////////

CStringsList::CStringsListNode* CStringsList::GetLastNode(bool CanReturnRoot)
{
	CStringsListNode *pNode = &Root;
	
	// Loop throught the nodes until we hit the last one
	while(pNode->Next != NULL)
		pNode = pNode->Next;

	if(CanReturnRoot)
		return pNode;

	// Return the last node pointer
	return pNode == &Root ? NULL : pNode;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CStringsList::Push(Strings string)
{
	Push(string.c_str());
}


/////////////////////////////////////////////////////////////////////////////////////////////////

void CStringsList::Push(char *string)
{
	CStringsListNode *pNode = GetLastNode(true);

	// Allocate the new node
	pNode->Next = new CStringsListNode;
	NumNodesAllocated++;
	CStringsListNode *pNewNode = pNode->Next;

	// Update pointers
	pNewNode->Next = NULL;
	pNewNode->Prev = pNode;

	//
	pNewNode->Indx = pNewNode->Prev->Indx + 1;

	// To customize...
	pNewNode->s = string;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CStringsList::Insert(Strings string, UINT Indx, BOOL InsAfter)
{
	return Insert(string.c_str(), Indx, InsAfter);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CStringsList::Insert(char* string, UINT Indx, BOOL InsAfter)
{
	// Update pointers
	if(InsAfter)
		Indx++;

	if(Indx >= GetNodesCount()){
		Push(string);
	} else {
		CStringsListNode *pNode = GetNode(Indx);
		if(!pNode){return false;}

		// Allocate the new node
		CStringsListNode *pNewNode = new CStringsListNode;
		NumNodesAllocated++;

		pNewNode->s = string;

		pNode->Prev->Next = pNewNode;
		pNewNode->Prev = pNode->Prev;
		pNewNode->Next = pNode;
		pNode->Prev = pNewNode;

		// To customize...

		RecreateIndex();
	}
	
	// return success
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CStringsList::Pop()
{
	CStringsListNode *pNode = GetLastNode(true);

	// If this the root node, we're done...
	if(pNode == NULL || pNode == &Root){return false;} // return faillure

	// Update pointers
	pNode->Prev->Next = NULL;

	// Delete the node
	SAFE_DELETE_OBJECT(pNode);
	NumNodesAllocated--;

	// return success
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CStringsList::Delete(UINT Indx)
{
	CStringsListNode *pNode = GetNode(Indx);

	// If this the root node, we're done...
	if(!pNode || pNode == &Root){return false;} // return faillure

	// Update pointers
	CStringsListNode *pPrevNode = pNode->Prev;
	CStringsListNode *pNextNode = pNode->Next;

	pPrevNode->Next = pNextNode;
	if(pNextNode)
		pNextNode->Prev = pPrevNode;

	// Delete the node
	SAFE_DELETE_OBJECT(pNode);
	NumNodesAllocated--;

	RecreateIndex();

	// return success
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CStringsList::Clear()
{
	// Delete everything...
	while(Pop());
}

/////////////////////////////////////////////////////////////////////////////////////////////////

bool CStringsList::LoadFromFile(char *pFileName)
{
	Clear();

	CTxtFileIO TxtFile;
	if(TxtFile.OpenForReading(pFileName, false)){

		CRawBuffer Buf(4096);
		char *pTxtLine = Buf.GetStrBuffer();

		while(TxtFile.ReadLine(Buf.GetBuffer(), Buf.GetBufferSize()) != NULL){

			// Erase the cariage return
			int Cpt = 0;
			while(pTxtLine[Cpt] != NULL && pTxtLine[Cpt] != 0x0A){
				Cpt++;
				if(Cpt == 4095){
					pTxtLine[Cpt] = NULL;
					break;
				}
			}

			if(pTxtLine[Cpt] == 0x0A)
				pTxtLine[Cpt] = NULL;
			
			this->Push(pTxtLine);
			#ifndef COMPILE_FOR_BORLAND
			Buf.Erase();
			#else
			Buf.EraseBuffer();
			#endif
		}

		TxtFile.Close();
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CStringsList::SaveToFile(char *pFileName)
{
	CTxtFileIO TxtFile;
	if(TxtFile.OpenForWriting(pFileName, false)){

		CStringsListNode *pNode = Root.Next;
		while(pNode != NULL){
			TxtFile.WriteLine(pNode->s.c_str());
			pNode = pNode->Next;
		}

		TxtFile.Close();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CStringsList::Print()
{
	if(Root.Next == NULL){return;}
	CStringsListNode *pNode = Root.Next;
	
	// Loop throught the nodes until we hit the last one
	while(pNode != NULL){
		printf(pNode->s.c_str());
		pNode = pNode->Next;
	}
}
