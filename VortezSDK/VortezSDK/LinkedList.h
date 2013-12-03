#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_
#ifdef __cplusplus
//----------------------------------------------------------------------//
#include <Windows.h>
//----------------------------------------------------------------------//

// Node 
template <class DataT> class CNode {
public:
	DataT Data;

	CNode(){
		Next = NULL;
		Prev = NULL;
	}
	CNode(const DataT& d){
	 : Data(d) // instead of default-constructing Data, use a copy-constructor
		Next = NULL;
		Prev = NULL;
	}

	CNode<DataT> *Next;
	CNode<DataT> *Prev;
	CNode<DataT> *GetNext(){return Next;}
	CNode<DataT> *GetPrev(){return Prev;}
};

//----------------------------------------------------------------------//

// Linked List 
template <class DataT> class CLinkedList : public CNode<DataT> {	
private:
	CNode<DataT> *First, *Last;
	UINT NumNodesAllocated;
public:
	CLinkedList(){
		First = NULL;
		Last  = NULL;
		NumNodesAllocated = 0;
	}
	~CLinkedList(){
		Clear();
	}
	 
	UINT GetNodesCount();

	CNode<DataT> *GetNode(UINT Indx);
	CNode<DataT> *GetFirstNode();
	CNode<DataT> *GetLastNode();

	CNode<DataT>* Push(const DataT *d);
	CNode<DataT>* PushBack(const DataT *d);
	CNode<DataT>* PushFront(const DataT *d);

	bool Pop(DataT *d);
	bool PopBack(DataT *d);
	bool PopFront(DataT *d);                                   // <- Pas Tester

	//CNode<DataT>* InsertBefore(CNode<DataT> *pNode, DataT *d); // <- Pas Tester
	//CNode<DataT>* InsertBefore(UINT Indx, DataT *d);           // <- Pas Tester

	//CNode<DataT>* InsertAfter(CNode<DataT> *pNode, DataT *d);  // <- Pas Tester
	//CNode<DataT>* InsertAfter(UINT Indx, DataT *d);            // <- Pas Tester

	//bool Delete(const DataT& d);
	bool Delete(UINT Indx);

	void Clear();

	void Randomize();

	void Fill(CNode<DataT> *pNode);
};

template <class DataT> UINT CLinkedList<DataT>::GetNodesCount()
{
	return NumNodesAllocated;
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::GetNode(UINT Indx)
{
	CNode<DataT> *pNode = First;
	if(!pNode){return NULL;}
	if(Indx >= NumNodesAllocated){return NULL;}

	UINT i = 0;
	while(i < Indx){
		pNode = pNode->GetNext();
		if(!pNode){return NULL;}
		i++;
	}

	return pNode;
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::GetFirstNode()
{
	return First;
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::GetLastNode()
{
	return Last;
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::Push(const DataT& d)
{
	CNode<DataT> *pNewNode = new CNode<DataT>;
	NumNodesAllocated++;

	pNewNode->Data = *d;
	if(First == NULL){
		Last  = pNewNode;
		First = pNewNode;
    } else {
		pNewNode->Prev = Last;
		Last->Next = pNewNode;
		Last = pNewNode;
    }

	return pNewNode;
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::PushBack(const DataT& d)
{
	return Push(d);
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::PushFront(const DataT& d)
{
	CNode<DataT> *pNewNode = new CNode<DataT>;
	NumNodesAllocated++;

	pNewNode->Data = *d;
	if(First == NULL){
		Last  = pNewNode;
		First = pNewNode;
    } else {
		pNewNode->Next = First;
		First->Prev = pNewNode;
		First = pNewNode;
    }

	return pNewNode;
}

template <class DataT> bool CLinkedList<DataT>::Pop(const DataT& d)
{
	if(Last){
		if(d != NULL)
			*d = Last->Data;
		return Delete(Last);
	}

	return false;
}

template <class DataT> bool CLinkedList<DataT>::PopBack(const DataT& d)
{
	return Pop(d);
}

template <class DataT> bool CLinkedList<DataT>::PopFront(const DataT& d)
{
	if(First){
		if(d != NULL)
			*d = First->Data;
		return Delete(First);
	}

	return false;
}

/*template <class DataT> CNode<DataT>* CLinkedList<DataT>::InsertBefore(CNode<DataT> *pNode, DataT *d)
{
	if(!pNode)
		return NULL;

	CNode<DataT> *pNewNode = new CNode<DataT>;
	NumNodesAllocated++;

	pNewNode->Data = *d;
	
	pNewNode->Next = pNode;
	pNewNode->Prev = pNode->Prev;

	if(pNode->Prev)
		pNode->Prev->Next = pNewNode;
	pNode->Prev = pNewNode;

	return pNewNode;
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::InsertBefore(UINT Indx, DataT *d)
{
	return InsertBefore(GetNode(Indx), d);
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::InsertAfter(CNode<DataT> *pNode, DataT *d)
{
	if(!pNode)
		return NULL;

	CNode<DataT> *pNewNode = new CNode<DataT>;
	NumNodesAllocated++;

	pNewNode->Data = *d;
	
	pNewNode->Prev = pNode;
	pNewNode->Next = pNode->Next;

	if(pNode->Next)
		pNode->Next->Prev = pNewNode;
	pNode->Next = pNewNode;

	return pNewNode;
}

template <class DataT> CNode<DataT>* CLinkedList<DataT>::InsertAfter(UINT Indx, DataT *d)
{
	return InsertAfter(GetNode(Indx), d);
}*/

template <class DataT> bool CLinkedList<DataT>::Delete(CNode<DataT> *pNode)
{
	if(pNode){
		if(pNode->Prev){
			pNode->Prev->Next = pNode->Next;
			if(pNode->Next){
				pNode->Next->Prev = pNode->Prev;
			} else {
				Last = pNode->Prev;
			}
		} else {
			if(pNode->Next){
				pNode->Next->Prev = NULL;
				First = pNode->Next;
			} else {
				First = NULL;
				Last  = NULL;
			}
		}

		delete pNode;
		NumNodesAllocated--;

		return true;
	}

	return false;
}

template <class DataT> bool CLinkedList<DataT>::Delete(UINT Indx)
{
	Delete(GetNode(Indx));
}

template <class DataT> void CLinkedList<DataT>::Clear()
{
	while(Pop(NULL));
}

template <class DataT> void CLinkedList<DataT>::Randomize()
{
	srand(GetTickCount());
	CLinkedList<DataT> TmpList;

	UINT NumItems = this->GetNodesCount();

	while(NumItems > 0){
	
		UINT RandVal = rand() % NumItems;
	
		CNode<DataT> *pNode = this->GetNode(RandVal);
		
		DataT d = pNode->Data;
		TmpList.Push(&d);

		this->Delete(pNode);

		NumItems--;
	}
	this->Clear();

	for(UINT Cpt = 0; Cpt < TmpList.GetNodesCount(); Cpt++){

		CNode<DataT> *pNode = TmpList.GetNode(Cpt);

		DataT d = pNode->Data;
		this->Push(&d);
	}

	TmpList.Clear();
}

template <class DataT> void CLinkedList<DataT>::Fill(CNode<DataT> *pNode)
{
	this->Clear();
	
	while(pNode){
		this->Push(&pNode->Data);
		pNode = pNode->GetNext();
	}
}

#endif
#endif //--_LINKED_LIST_H_
