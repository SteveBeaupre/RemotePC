#include "Bits.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// Return information about the bit position into the bitfield
////////////////////////////////////////////////////////////////////////////////////////////////
BYTE* Setup(void *pBitField, DWORD BitIndx, DWORD *pBitShift)
{
	BYTE *pData = (BYTE*)pBitField;
	pData += BitIndx / 8;
	*pBitShift = BitIndx % 8;
	return pData;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Return true if the bit is set to 1
////////////////////////////////////////////////////////////////////////////////////////////////
bool ReadBit(void *pBitField, DWORD BitIndx, DWORD DataSize)
{
	// Make sure we aren't testing beyond the data memory limit
	if(DataSize > 0 && (BitIndx/8)+1 > DataSize)
		return false;

	// Tell how many bits to shift in the byte pointer below
	DWORD BitShift = 0;
	// Make a BYTE pointer to simplify things...
	BYTE *pData = Setup(pBitField, BitIndx, &BitShift);
	// Return true if the bit is set to 1
	return ((*pData >> BitShift) & 0x01) > 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Write 0 or 1 into the bitfield 
////////////////////////////////////////////////////////////////////////////////////////////////
void WriteBit(void *pBitField, DWORD BitIndx, BYTE BitValue, DWORD DataSize)
{
	// Make sure we aren't writing beyond the data memory limit
	if(DataSize > 0 && (BitIndx/8)+1 > DataSize)
		return;

	// Make sure the value here is only 0 or 1
	BitValue = (BitValue & 0x01);

	// Tell how many bits to shift in the byte pointer below
	DWORD BitShift = 0;
	// Make a BYTE pointer to simplify things...
	BYTE *pData = Setup(pBitField, BitIndx, &BitShift);

	// Set the bit to 0 or 1
	switch(BitValue)
	{
	case 1:	 *pData |=  (0x01 << BitShift); break;
	default: *pData &= ~(0x01 << BitShift); break;
	}
}
