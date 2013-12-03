#include "CRC32.h"

CCRC32::CCRC32()
{
	g_crc = 0xFFFFFFFF;
	GenTab();
}

void CCRC32::GenTab()
{
	DWORD crc, poly = 0xEDB88320L;
	for(int i = 0; i < 256; i++){
		crc = i;
		for(int j = 8; j > 0; j--){
			crc = (crc & 1) ? ((crc >> 1) ^ poly) : (crc >> 1);
		}
		tab[i] = crc;
	}
}

DWORD CCRC32::Generate(void *pBlock, DWORD BlockLen)
{
	BYTE *Block = (BYTE*)pBlock;
	register DWORD crc;
	crc = 0xFFFFFFFF;
	for(DWORD i = 0; i < BlockLen; i++)
		crc = ((crc >> 8) & 0x00FFFFFF) ^ tab[(crc ^ *Block++) & 0xFF];
	
	return (crc ^ 0xFFFFFFFF);
}

#ifndef COMPILE_FOR_BORLAND
DWORD CCRC32::Generate(DWORD Op, void *pBlock, DWORD BlockLen)
#else
DWORD CCRC32::GenerateExt(DWORD Op, void *pBlock, DWORD BlockLen)
#endif
{
	BYTE *Block = (BYTE*)pBlock;
	if(Op == CRC32_INIT){
		g_crc = 0xFFFFFFFF;
	} else if(Op == CRC32_FEED){
		register DWORD crc = g_crc;
		for(DWORD i = 0; i < BlockLen; i++)
			crc = ((crc >> 8) & 0x00FFFFFF) ^ tab[(crc ^ *Block++) & 0xFF];
		g_crc = crc;
	}
	
	return g_crc ^ 0xFFFFFFFF;
}
