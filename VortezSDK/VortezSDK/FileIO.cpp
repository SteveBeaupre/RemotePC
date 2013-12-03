#include "FileIO.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor...
////////////////////////////////////////////////////////////////////////////////////////////////
CFileIO::CFileIO()
{
	hFile = INVALID_HANDLE_VALUE;
}

CFileIO::~CFileIO()
{
	this->Close();
}

bool CFileIO::IsOpened()
{
	return hFile != INVALID_HANDLE_VALUE;
}


// Open a file for reading and writing if CanWrite is set. 
// The file must exist.
bool CFileIO::OpenForReading(char *fname, bool CanWrite)
{
	this->Close();

	switch(CanWrite)
	{
	case false: hFile = CreateFile(fname, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); break;
	case true:  hFile = CreateFile(fname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); break;
	}
	
	return IsOpened();
}

// Create an empty file for writing and reading if CanRead is set. 
// If a file with the same name already exists its content is erased and 
// the file is treated as a new empty file. 
bool CFileIO::OpenForWriting(char *fname, bool CanRead)
{
	this->Close();

	switch(CanRead)
	{
	case false: hFile = CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); break;
	case true:  hFile = CreateFile(fname, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); break;
	}
	
	return IsOpened();
}


// Open a file for appending and reading if CanRead is set. 
// All writing operations are performed at the end of the file, 
// protecting the previous content to be overwritten. 
// You can reposition (fseek, rewind) the internal pointer to anywhere in 
// the file for reading, but writing operations will move it back to the end of file. 
// The file is created if it does not exist.
bool CFileIO::OpenForAppending(char *fname, bool CanRead)
{
	this->Close();

	switch(CanRead)
	{
	case false: hFile = CreateFile(fname, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); break;
	case true:  hFile = CreateFile(fname, FILE_APPEND_DATA | FILE_READ_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); break;
	}
	
	return IsOpened();
}


void CFileIO::Close()
{
	if(IsOpened()){
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
}

UI64 CFileIO::GetSize()
{
	if(!IsOpened()){return 0;}

	DWORD SizeHi = 0;
	DWORD SizeLo = GetFileSize(hFile, &SizeHi);
	
	return DWORDX2ToUI64(SizeHi, SizeLo);
}

void CFileIO::Flush()
{
	if(IsOpened())
		FlushFileBuffers(hFile);
}

UI64 CFileIO::Tell()
{
	if(!IsOpened())
		return 0;

	LARGE_INTEGER Pos = UI64ToLARGE_INTEGER(0);
	Pos.LowPart = SetFilePointer(hFile, Pos.LowPart, &Pos.HighPart, FILE_CURRENT);

	if(Pos.LowPart == 0xFFFFFFFF && (GetLastError() != NO_ERROR))
		return 0;

	return LARGE_INTEGERToUI64(Pos);
}

void CFileIO::Seek(UI64 uiDistanceToMove, DWORD dwMoveMethod)
{
	LARGE_INTEGER liDistanceToMove = UI64ToLARGE_INTEGER(uiDistanceToMove);
	this->Seek(liDistanceToMove, dwMoveMethod);
}

void CFileIO::Seek(LARGE_INTEGER liDistanceToMove, DWORD dwMoveMethod)
{
	if(!IsOpened()){return;}

	SetFilePointer(hFile, liDistanceToMove.LowPart, &liDistanceToMove.HighPart, dwMoveMethod);
}

UINT CFileIO::Read(void *pBuffer, UINT NumBytesToRead)
{
	if(!IsOpened()){return 0;}

	UI64 FileSize = GetSize();
	DWORD NumBytesRead = 0;

	if(Tell() < FileSize)
		ReadFile(hFile, pBuffer, NumBytesToRead, &NumBytesRead, NULL);	

	return NumBytesRead;
}

UINT CFileIO::Write(void *pBuffer, UINT NumBytesToWrite)
{
	if(!IsOpened()){return 0;}

	DWORD NumBytesWriten = 0;
	WriteFile(hFile, pBuffer, NumBytesToWrite, &NumBytesWriten, NULL);	

	return NumBytesWriten;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

CTxtFileIO::CTxtFileIO()
{
	hFile = NULL;
}

CTxtFileIO::~CTxtFileIO()
{
	this->Close();
}

bool CTxtFileIO::IsOpened()
{
	return hFile != NULL;
}


bool CTxtFileIO::OpenForReading(char *fname, bool CanWrite)
{
	// Make sure there not a file already opened
	if(hFile){this->Close();}
	
	// Flags parameters
	char szFlags[4] = {0,0,0,0};

	strcat(&szFlags[0], "r");
	if(CanWrite)
		strcat(&szFlags[0], "+");

	hFile = fopen(fname, &szFlags[0]);

	return hFile != NULL;
}

bool CTxtFileIO::OpenForWriting(char *fname, bool CanRead)
{
	// Make sure there not a file already opened
	if(hFile){this->Close();}
	
	// Flags parameters
	char szFlags[4] = {0,0,0,0};

	strcat(&szFlags[0], "w");
	if(CanRead)
		strcat(&szFlags[0], "+");

	hFile = fopen(fname, &szFlags[0]);

	return hFile != NULL;
}

bool CTxtFileIO::OpenForAppending(char *fname, bool CanRead)
{
	// Make sure there not a file already opened
	if(hFile){this->Close();}
	
	// Flags parameters
	char szFlags[4] = {0,0,0,0};

	strcat(&szFlags[0], "a");
	if(CanRead)
		strcat(&szFlags[0], "+");

	hFile = fopen(fname, &szFlags[0]);

	return hFile != NULL;
}

void CTxtFileIO::Close()
{
	if(hFile){
		fclose(hFile);
		hFile = NULL;		
	}
}

void CTxtFileIO::Flush()
{
	if(hFile)
		fflush(hFile);
}

UINT CTxtFileIO::Tell()
{
	if(!hFile){return 0;}
	return ftell(hFile);
}

void CTxtFileIO::Seek(DWORD DistanceToMove, DWORD MoveMethod)
{
	if(!hFile){return;}
	fseek(hFile, DistanceToMove, MoveMethod);
}

char* CTxtFileIO::ReadLine(void *pBuffer, UINT BufferSize)
{
	if(!hFile){return NULL;}
	return fgets((char*)pBuffer, BufferSize, hFile);
}

int CTxtFileIO::WriteLine(void *pBuffer)
{
	if(!hFile){return 0;}
	return fputs((char*)pBuffer, hFile);
}

int CTxtFileIO::WriteArgs(const char *fmt, ...)
{
	if(fmt == NULL)
		return 0;

	const int BufSize = 0x1000;

	char TextBuf[BufSize];
	char *pTxt = &TextBuf[0];
	ZeroMemory(pTxt, BufSize);

	va_list ap;
	va_start(ap, fmt);
	_vsnprintf(pTxt, BufSize, fmt, ap);
	va_end(ap);

	return WriteLine(pTxt);
}

bool CTxtFileIO::ReadChar(char *c)
{
	if(!hFile){return false;}
	int ic = fgetc(hFile);
	if(ic != EOF)
		*c = (char)ic;
	return ic != EOF;
}

bool CTxtFileIO::WriteChar(char c)
{
	if(!hFile){return false;}
	return (char)fputc(c, hFile) != EOF;
}

bool CTxtFileIO::PutBackChar(char c)
{
	if(!hFile){return false;}
	return ungetc(c, hFile) != EOF;
}
