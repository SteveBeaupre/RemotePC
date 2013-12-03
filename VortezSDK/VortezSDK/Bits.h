#ifndef _CBITS_H_
#define _CBITS_H_
#ifdef __cplusplus

#include <Windows.h>
#include "PreProc.h"

bool _EXP_FUNC ReadBit(void *pBitField, DWORD BitIndx, DWORD DataSize = 0);
void _EXP_FUNC WriteBit(void *pBitField, DWORD BitIndx, BYTE BitValue = 1, DWORD DataSize = 0);

#endif
#endif //_CBITS_H_/*