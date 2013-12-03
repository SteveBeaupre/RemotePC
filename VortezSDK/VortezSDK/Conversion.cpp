#include "Conversion.h"

bool IntToBool(int i)
{
	return i == 0;
}

int BoolToInt(bool b)
{
	return b == false ? 0 : 1;
}


LARGE_INTEGER DWORDToLARGE_INTEGER(DWORD dw)
{
	LARGE_INTEGER li;
	li.HighPart = 0;
	li.LowPart  = dw;
	return li;
}

LARGE_INTEGER UI64ToLARGE_INTEGER(UI64 ui)
{
	LARGE_INTEGER li;
	li.QuadPart = ui;
	return li;
}

UI64 DWORDToUI64(DWORD dw)
{
	UI64 ui = dw;
	return ui;
}

UI64 DWORDX2ToUI64(DWORD dwHi, DWORD dwLo)
{
	UI64 ui = dwHi;
	ui <<= 32;
	ui |= dwLo;
	return ui;
}

UI64 LARGE_INTEGERToUI64(LARGE_INTEGER li)
{
	UI64 ui = li.QuadPart;
	return ui;
}

UI64 SectorsToBytes(UI64 NumSectors, DWORD SectorSize)
{
	UI64 Res = NumSectors;
	Res *= SectorSize;
	return Res;
}

UI64 BytesToSectors(UI64 NumBytes, DWORD SectorSize)
{
	UI64 Res = NumBytes;
	Res /= SectorSize;
	return Res;
}

DWORD LODWORD(UI64 qword)
{
	DWORD Res = (DWORD)qword;
	return Res;
}

DWORD HIDWORD(UI64 qword)
{
	DWORD Res = (DWORD)(qword >> 32);
	return Res;
}

