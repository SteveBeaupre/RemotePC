#ifndef _RAW_BUFFER_H_
#define _RAW_BUFFER_H_
#ifdef __cplusplus

#include "Preproc.h"

#include "Windows.h"
#include "SafeKill.h"

#include "FileIO.h"
#include "FileSystem.h"

#ifdef COMPILE_FOR_BORLAND
class ARawBuffer {
public:
	virtual bool  _INTERNAL_LINKAGE IsAllocated() = 0;
	
	virtual BYTE* _INTERNAL_LINKAGE GetBuffer(UINT Indx = 0) = 0;
	virtual char* _INTERNAL_LINKAGE GetStrBuffer(UINT Indx = 0) = 0;
	
	virtual UINT  _INTERNAL_LINKAGE GetBufferSize() = 0;
	virtual UINT  _INTERNAL_LINKAGE GetStrBufferSize(UINT Indx = 0) = 0;

	virtual bool  _INTERNAL_LINKAGE AllocateBuffer(UINT Size) = 0;
	virtual void  _INTERNAL_LINKAGE EraseBuffer() = 0;
	virtual void  _INTERNAL_LINKAGE SetBufferBytes(BYTE Val) = 0;
	virtual void  _INTERNAL_LINKAGE FreeBuffer() = 0;
	
	virtual void  _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CRawBuffer : public ARawBuffer {
#else
class _EXP_FUNC CRawBuffer {
#endif
public:
	CRawBuffer();
	CRawBuffer(UINT Size);
	~CRawBuffer();
private:
	UINT BufSize;
	BYTE *pBuf;
	bool OverflowCheck(UINT Offset);
public:
	bool  _INTERNAL_LINKAGE IsAllocated();
	
	BYTE* _INTERNAL_LINKAGE GetBuffer(UINT Indx = 0);
	char* _INTERNAL_LINKAGE GetStrBuffer(UINT Indx = 0);
	
	UINT  _INTERNAL_LINKAGE GetSize();
	UINT  _INTERNAL_LINKAGE GetBufferSize(){return GetSize();}
	UINT  _INTERNAL_LINKAGE GetStrBufferSize(UINT Indx = 0);
	
#ifndef COMPILE_FOR_BORLAND
	bool  _INTERNAL_LINKAGE Allocate(UINT Size, BOOL EraseBuf = FALSE);
	void  _INTERNAL_LINKAGE Erase();
	void  _INTERNAL_LINKAGE Set(BYTE Val);
	void  _INTERNAL_LINKAGE Free();
#else
	bool  _INTERNAL_LINKAGE AllocateBuffer(UINT Size);
	void  _INTERNAL_LINKAGE EraseBuffer();
	void  _INTERNAL_LINKAGE SetBufferBytes(BYTE Val);
	void  _INTERNAL_LINKAGE FreeBuffer();

	void  _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif

	bool  _INTERNAL_LINKAGE SaveToFile(char *fname, UINT size = 0);
	bool  _INTERNAL_LINKAGE LoadFromFile(char *fname);
};

#endif
#endif //_RAW_BUFFER_H_


