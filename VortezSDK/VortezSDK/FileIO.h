#ifndef _CFILEIO_H_
#define _CFILEIO_H_
#ifdef __cplusplus

#include "Preproc.h"

#include <Windows.h>
#include <stdio.h>

#include "Bits.h"
#include "Conversion.h"

#ifdef COMPILE_FOR_BORLAND
class AFileIO {
public:
	virtual bool _INTERNAL_LINKAGE IsOpened() = 0;
	virtual bool _INTERNAL_LINKAGE OpenForReading(char *fname, bool CanWrite = false) = 0;
	virtual bool _INTERNAL_LINKAGE OpenForWriting(char *fname, bool CanRead = false) = 0;
	virtual bool _INTERNAL_LINKAGE OpenForAppending(char *fname, bool CanRead = false) = 0;
	virtual void _INTERNAL_LINKAGE Close() = 0;

	virtual UI64 _INTERNAL_LINKAGE GetSize() = 0;
	virtual void _INTERNAL_LINKAGE Flush() = 0;

	virtual UI64 _INTERNAL_LINKAGE Tell() = 0;
	virtual void _INTERNAL_LINKAGE Seek(UI64 uiDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN) = 0;

	// Binary buffer Read/Write
	virtual UINT _INTERNAL_LINKAGE Read(void *pBuffer, UINT NumBytesToRead) = 0;
	virtual UINT _INTERNAL_LINKAGE Write(void *pBuffer, UINT NumBytesToWrite) = 0;
	
	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CFileIO : public AFileIO {
#else
class _EXP_FUNC CFileIO {
#endif
public:
	CFileIO();
	~CFileIO();
private:
	HANDLE hFile;
public:
	// Open/Create/Close ops.
	bool _INTERNAL_LINKAGE IsOpened();
	bool _INTERNAL_LINKAGE OpenForReading(char *fname, bool CanWrite = false);
	bool _INTERNAL_LINKAGE OpenForWriting(char *fname, bool CanRead = false);
	bool _INTERNAL_LINKAGE OpenForAppending(char *fname, bool CanRead = false);
	void _INTERNAL_LINKAGE Close();

	UI64 _INTERNAL_LINKAGE GetSize();
	void _INTERNAL_LINKAGE Flush();

	UI64 _INTERNAL_LINKAGE Tell();
	void _INTERNAL_LINKAGE Seek(UI64 uiDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN);
	void _INTERNAL_LINKAGE Seek(LARGE_INTEGER liDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN);

	// Binary buffer Read/Write
	UINT _INTERNAL_LINKAGE Read(void *pBuffer, UINT NumBytesToRead);
	UINT _INTERNAL_LINKAGE Write(void *pBuffer, UINT NumBytesToWrite);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#ifdef COMPILE_FOR_BORLAND
class ATxtFileIO {
public:
	// Open/Create/Close ops.
	virtual bool _INTERNAL_LINKAGE IsOpened() = 0;
	virtual bool _INTERNAL_LINKAGE OpenForReading(char *fname, bool CanWrite = false) = 0;
	virtual bool _INTERNAL_LINKAGE OpenForWriting(char *fname, bool CanRead = false) = 0;
	virtual bool _INTERNAL_LINKAGE OpenForAppending(char *fname, bool CanRead = false) = 0;
	virtual void _INTERNAL_LINKAGE Close() = 0;

	virtual void _INTERNAL_LINKAGE Flush() = 0;
	virtual UINT _INTERNAL_LINKAGE Tell() = 0;
	virtual void _INTERNAL_LINKAGE Seek(DWORD DistanceToMove, DWORD MoveMethod = SEEK_SET) = 0;
	
	// Text buffer Read/Write
	virtual bool  _INTERNAL_LINKAGE ReadChar(char *c) = 0;
	virtual char* _INTERNAL_LINKAGE ReadLine(void *pBuffer, UINT BufferSize) = 0;
	virtual int   _INTERNAL_LINKAGE WriteLine(void *pBuffer) = 0;
	virtual int   _INTERNAL_LINKAGE WriteArgs(const char *fmt, ...) = 0;
	// Char. Read/Write ops.
	virtual bool _INTERNAL_LINKAGE WriteChar(char c) = 0;
	virtual bool _INTERNAL_LINKAGE PutBackChar(char c) = 0;
	
	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CTxtFileIO : public ATxtFileIO {
#else
class _EXP_FUNC CTxtFileIO {
#endif
public:
	CTxtFileIO();
	~CTxtFileIO();
private:
	FILE *hFile;
public:
	// Open/Create/Close ops.
	bool _INTERNAL_LINKAGE IsOpened();
	bool _INTERNAL_LINKAGE OpenForReading(char *fname, bool CanWrite = false);
	bool _INTERNAL_LINKAGE OpenForWriting(char *fname, bool CanRead = false);
	bool _INTERNAL_LINKAGE OpenForAppending(char *fname, bool CanRead = false);
	void _INTERNAL_LINKAGE Close();

	void _INTERNAL_LINKAGE Flush();
	UINT _INTERNAL_LINKAGE Tell();
	void _INTERNAL_LINKAGE Seek(DWORD DistanceToMove, DWORD MoveMethod = SEEK_SET);
	
	// Text buffer Read/Write
	bool  _INTERNAL_LINKAGE ReadChar(char *c);
	char* _INTERNAL_LINKAGE ReadLine(void *pBuffer, UINT BufferSize);
	int   _INTERNAL_LINKAGE WriteLine(void *pBuffer);
	int   _INTERNAL_LINKAGE WriteArgs(const char *fmt, ...);
	// Char. Read/Write ops.
	bool _INTERNAL_LINKAGE WriteChar(char c);
	bool _INTERNAL_LINKAGE PutBackChar(char c);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_CFILEIO_H_
