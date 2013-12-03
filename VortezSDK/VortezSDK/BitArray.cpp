#include "BitArray.h"

CBitArray::CBitArray()
{
	pBuffer = NULL;
	BufferSize = 0;
	BitsCount  = 0;
}

CBitArray::~CBitArray()
{
	FreeMem();
}

bool CBitArray::IsAllocated()
{
	return pBuffer != NULL;
}

void CBitArray::Allocate(UINT NumBits)
{
	if(IsAllocated() || NumBits == 0){return;}

	BitsCount  = NumBits;
	BufferSize = ((NumBits-1)/8)+1;

	pBuffer = new BYTE[BufferSize];
	ClearAll();
}

void CBitArray::Resize(UINT NumBits)
{
	if(!IsAllocated() || NumBits == 0){return;}

	BYTE *pTmpBuffer = new BYTE[BufferSize];
	memcpy(pTmpBuffer, pBuffer, BufferSize);

	UINT NewBitsCount  = NumBits;
	UINT NewBufferSize = ((NumBits-1)/8)+1;

	delete [] pBuffer;
	pBuffer = NULL;
	pBuffer = new BYTE[NewBufferSize];
	
	ZeroMemory(pBuffer, NewBufferSize);
	if(NewBufferSize >= BufferSize){
		memcpy(pBuffer, pTmpBuffer, BufferSize);
	} else {
		memcpy(pBuffer, pTmpBuffer, NewBufferSize);
	}

	BitsCount  = NewBitsCount;
	BufferSize = NewBufferSize;

	delete [] pTmpBuffer;
}

void CBitArray::FreeMem()
{
	if(IsAllocated()){
		delete [] pBuffer;
		pBuffer = NULL;
		BufferSize = 0;
		BitsCount  = 0;
	}
}

#ifndef COMPILE_FOR_BORLAND
bool CBitArray::operator[](UINT Indx)
{
	if(!IsAllocated() || Indx >= BitsCount){return false;}
	return ReadBit(pBuffer, Indx, BufferSize);
}
#endif

void CBitArray::ClearAll()
{
	if(!IsAllocated()){return;}
	ZeroMemory(pBuffer, BufferSize);
}

void CBitArray::SetAll()
{
	if(!IsAllocated()){return;}
	memset(pBuffer, 1, BufferSize);
}

bool CBitArray::GetBit(UINT Indx)
{
	if(!IsAllocated() || Indx >= BitsCount){return false;}
	return ReadBit(pBuffer, Indx, BufferSize);
}

void CBitArray::SetBit(UINT Indx, bool Value)
{
	if(!IsAllocated() || Indx >= BitsCount){return;}
	WriteBit(pBuffer, Indx, Value, BufferSize);
}
