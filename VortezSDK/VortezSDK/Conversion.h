#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include "PreProc.h"

#include <Windows.h>

#define UI64  unsigned __int64

bool _EXP_FUNC IntToBool(int i);
int  _EXP_FUNC BoolToInt(bool b);

LARGE_INTEGER _EXP_FUNC DWORDToLARGE_INTEGER(DWORD dw);
UI64          _EXP_FUNC DWORDToUI64(DWORD dw);

UI64 _EXP_FUNC DWORDX2ToUI64(DWORD dwHi, DWORD dwLo);

LARGE_INTEGER _EXP_FUNC UI64ToLARGE_INTEGER(UI64 ui);
UI64          _EXP_FUNC LARGE_INTEGERToUI64(LARGE_INTEGER li);

UI64 _EXP_FUNC SectorsToBytes(UI64 NumSectors, DWORD SectorSize);
UI64 _EXP_FUNC BytesToSectors(UI64 NumBytes, DWORD SectorSize);

#ifndef RAD_STUDIO_XE
DWORD _EXP_FUNC LODWORD(UI64 qword);
DWORD _EXP_FUNC HIDWORD(UI64 qword);
#endif

#endif //--_CONVERSION_H_