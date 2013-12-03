#ifndef _CRC32_H_
#define _CRC32_H_
#ifdef __cplusplus

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "Preproc.h"

#define CRC32_INIT  0
#define CRC32_FEED  1
#define CRC32_GET   2

#ifdef COMPILE_FOR_BORLAND
class ACRC32 {
public:
	virtual DWORD _INTERNAL_LINKAGE Generate(void *pBlock, DWORD BlockLen) = 0;
	virtual DWORD _INTERNAL_LINKAGE GenerateExt(DWORD Op, void *pBlock, DWORD BlockLen) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CCRC32  : public ACRC32 {
#else
class _EXP_FUNC CCRC32 {
#endif
public:
	CCRC32();
private:
	DWORD g_crc;
	DWORD tab[256];
	void GenTab();
public:
	DWORD _INTERNAL_LINKAGE Generate(void *block, DWORD length);
#ifndef COMPILE_FOR_BORLAND
	DWORD _INTERNAL_LINKAGE Generate(DWORD Op, void *block, DWORD length);
#else
	DWORD _INTERNAL_LINKAGE GenerateExt(DWORD Op, void *block, DWORD length);
#endif

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //--_CRC32_H_
