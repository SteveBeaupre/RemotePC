#ifndef _CSTRINGSLIST_H_
#define _CSTRINGSLIST_H_
#ifdef __cplusplus

#include "Windows.h"

#include "Strings.h"
#include "FileIO.h"
#include "RawBuffer.h"
#include "SafeKill.h"

#include "Preproc.h"

#ifdef COMPILE_FOR_BORLAND
// Create an abstract class of the object to export
class AStringsList {
public:

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CStringsList : public AStringsList
#else
class _EXP_FUNC CStringsList 
#endif
{
public:
	#ifdef COMPILE_FOR_BORLAND
	struct CStringsListNode {
	#else
	struct _EXP_FUNC CStringsListNode {
	#endif
		UINT Indx;
		Strings s;
		struct CStringsListNode *Prev;
		struct CStringsListNode *Next;
	};
public:
	CStringsList();
	~CStringsList();
private:
	UINT NumNodesAllocated;
	
	CStringsListNode  Root;
	CStringsListNode* GetRootNode();
	//CStringsListNode* pLastNode;

	void InitRootNode();
	void RecreateIndex();
public:
	CStringsListNode* GetNode(UINT Indx);
	CStringsListNode* GetFirstNode(bool CanReturnRoot = false);
	CStringsListNode* GetLastNode(bool CanReturnRoot = false);
	
	Strings* GetStringPointer(UINT Indx);
	UINT GetNodesCount();

	void Push(char *string);
	void Push(Strings string);
	bool Insert(char *string, UINT Indx, BOOL InsAfter = false);
	bool Insert(Strings string, UINT Indx, BOOL InsAfter = false);
	bool Pop();
	bool Delete(UINT Indx);
	void Clear();

	bool LoadFromFile(char *pFileName);
	void SaveToFile(char *pFileName);

	void Print();

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_CSTRINGSLIST_H_/*