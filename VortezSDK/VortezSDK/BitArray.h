#ifndef _CBITARRAY_H_
#define _CBITARRAY_H_
#ifdef __cplusplus

#include <Windows.h>
#include "Bits.h"

#include "Preproc.h"

#ifdef COMPILE_FOR_BORLAND
// Create an abstract class of the object to export
class ABitArray {
public:
	virtual bool _INTERNAL_LINKAGE IsAllocated() = 0;
	virtual void _INTERNAL_LINKAGE Allocate(UINT NumBits) = 0;
	virtual void _INTERNAL_LINKAGE Resize(UINT NumBits) = 0;
	virtual void _INTERNAL_LINKAGE FreeMem() = 0;

	virtual void _INTERNAL_LINKAGE ClearAll() = 0;
	virtual void _INTERNAL_LINKAGE SetAll() = 0;

	virtual bool _INTERNAL_LINKAGE GetBit(UINT Indx) = 0;
	virtual void _INTERNAL_LINKAGE SetBit(UINT Indx, bool Value = true) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CBitArray : public ABitArray {
#else
class _EXP_FUNC CBitArray {
#endif
public:
	CBitArray();
	~CBitArray();
private:
	BYTE *pBuffer;
	UINT BitsCount;
	UINT BufferSize;
public:
	bool _INTERNAL_LINKAGE IsAllocated();
	void _INTERNAL_LINKAGE Allocate(UINT NumBits);
	void _INTERNAL_LINKAGE Resize(UINT NumBits);
	void _INTERNAL_LINKAGE FreeMem();

	void _INTERNAL_LINKAGE ClearAll();
	void _INTERNAL_LINKAGE SetAll();

	bool _INTERNAL_LINKAGE operator[](UINT Indx);
	bool _INTERNAL_LINKAGE GetBit(UINT Indx);
	void _INTERNAL_LINKAGE SetBit(UINT Indx, bool Value = true);
	
#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_CBITARRAY_H_/*