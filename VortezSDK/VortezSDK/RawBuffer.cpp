#include "RawBuffer.h"

CRawBuffer::CRawBuffer()
{
	BufSize = 0; 
	pBuf = NULL;
}

CRawBuffer::CRawBuffer(UINT Size)
{
	BufSize = 0; 
	pBuf = NULL; 
#ifndef COMPILE_FOR_BORLAND
	Allocate(Size);
#else 
	AllocateBuffer(Size);
#endif
}

CRawBuffer::~CRawBuffer()
{
#ifndef COMPILE_FOR_BORLAND
	Free();
#else 
	FreeBuffer();
#endif
}

bool CRawBuffer::OverflowCheck(UINT Offset)
{
	return Offset >= BufSize;
}

bool CRawBuffer::IsAllocated()
{
	return pBuf != NULL;
}

BYTE* CRawBuffer::GetBuffer(UINT Offset){
	if(OverflowCheck(Offset)){return NULL;}
	return &pBuf[Offset];
}

char* CRawBuffer::GetStrBuffer(UINT Offset)
{
	if(OverflowCheck(Offset)){return NULL;}
	return (char*)&pBuf[Offset];
}

UINT CRawBuffer::GetSize()
{
	return BufSize;
}

UINT CRawBuffer::GetStrBufferSize(UINT Offset)
{
	if(OverflowCheck(Offset)){return 0;}
	if(IsAllocated()){
		return strlen((char*)&pBuf[Offset]);
	} else {
		return 0;
	}
}

#ifndef COMPILE_FOR_BORLAND
bool CRawBuffer::Allocate(UINT Size, BOOL EraseBuf)
#else
bool CRawBuffer::AllocateBuffer(UINT Size, BOOL EraseBuf)
#endif
{
	if(IsAllocated()){
		if(BufSize != Size){
			Free();
		} else {
			#ifndef COMPILE_FOR_BORLAND
			if(EraseBuf)
				Erase();
			#else
			if(EraseBuf)
				EraseBuffer();
			#endif
			return true;
		}
	}

	if(!IsAllocated()){
		pBuf = new BYTE[Size]; 
		if(IsAllocated()){
			BufSize = Size; 
			#ifndef COMPILE_FOR_BORLAND
			Erase(); 
			#else
			EraseBuffer(); 
			#endif
			return true;
		}
	}

	return false;
}

#ifndef COMPILE_FOR_BORLAND
void CRawBuffer::Erase()
#else
void CRawBuffer::EraseBuffer()
#endif
{
	if(IsAllocated())
		ZeroMemory(pBuf, BufSize);
}

#ifndef COMPILE_FOR_BORLAND
void CRawBuffer::Set(BYTE Val)
#else
void CRawBuffer::SetBufferBytes(BYTE Val)
#endif
{
	if(IsAllocated())
		memset(pBuf, Val, BufSize);
}

#ifndef COMPILE_FOR_BORLAND
void CRawBuffer::Free()
#else
void CRawBuffer::FreeBuffer()
#endif
{
	if(IsAllocated()){
		SAFE_DELETE_ARRAY(pBuf); 
		BufSize = 0;
	}
}

bool CRawBuffer::SaveToFile(char *fname, UINT size)
{
	if(size == 0)
		size = BufSize;

	CFileIO f;
	if(f.OpenForWriting(fname)){
		
		if(size > 0)
			f.Write(pBuf, size);	

		f.Close();

		return true;
	}

	return false;
}

bool CRawBuffer::LoadFromFile(char *fname)
{
	CFileManager FileManager;
	if(!FileManager.DoesExist(fname))
		return false;

	UINT fSize = FileManager.GetSize(fname);
	if(fSize == 0)
		return false;

	CFileIO f;
	if(f.OpenForReading(fname)){
		
		Allocate(fSize);
		f.Read(pBuf, fSize);	

		f.Close();

		return true;
	}

	return false;
}
