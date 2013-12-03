#ifndef _CDRIVEIO_H_
#define _CDRIVEIO_H_
#ifdef __cplusplus

#include "Preproc.h"

#include "Windows.h"
#include "stdio.h"
#include "Conversion.h"

#ifdef COMPILE_FOR_BORLAND
class ADriveIO {
public:
	virtual bool _INTERNAL_LINKAGE IsOpened() = 0;
	virtual bool _INTERNAL_LINKAGE Open(char Drive, bool WriteAccess = false) = 0;
	virtual void _INTERNAL_LINKAGE Close() = 0;

	virtual UI64 _INTERNAL_LINKAGE Tell() = 0;
	virtual void _INTERNAL_LINKAGE Seek(LARGE_INTEGER liDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN) = 0;
	virtual void _INTERNAL_LINKAGE Seek(UI64 uiDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN) = 0;

	virtual DWORD _INTERNAL_LINKAGE ReadBuffer(BYTE *pBuffer, UINT NumBytesToRead) = 0;
	virtual DWORD _INTERNAL_LINKAGE ReadSectors(BYTE *pBuffer, UINT NumSectorsToRead) = 0;
	virtual DWORD _INTERNAL_LINKAGE WriteBuffer(BYTE *pBuffer, UINT NumBytesToWrite) = 0;
	virtual DWORD _INTERNAL_LINKAGE WriteSectors(BYTE *pBuffer, UINT NumSectorsToWrite) = 0;

	virtual char _INTERNAL_LINKAGE GetOpenedDriveLetter() = 0;

	virtual UI64 _INTERNAL_LINKAGE GetSize() = 0;
	virtual UI64 _INTERNAL_LINKAGE GetSectorsCount() = 0;

	virtual DWORD _INTERNAL_LINKAGE GetNumberOfClusters() = 0;
	virtual DWORD _INTERNAL_LINKAGE GetNumBytesPerSector() = 0;
	virtual DWORD _INTERNAL_LINKAGE GetNumSectorsPerCluster() = 0;

	virtual UI64 _INTERNAL_LINKAGE GetNumBytesFree(char Drive) = 0;
	virtual UI64 _INTERNAL_LINKAGE GetNumBytesUsed(char Drive) = 0;
	virtual UI64 _INTERNAL_LINKAGE GetNumSectorFree(char Drive) = 0;
	virtual UI64 _INTERNAL_LINKAGE GetNumSectorUsed(char Drive) = 0;
	
	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CDriveIO  : public ADriveIO {
#else
class _EXP_FUNC CDriveIO {
#endif
public:
	CDriveIO();
private:
	HANDLE hDrive;

	char  DriveLetter;
	UI64  CursorPos;

	UI64  NumBytes;
	UI64  NumSectors;
	UI64  SectorSize;
	
	DWORD NumBytesPerSector;
	DWORD NumSectorsPerCluster;
	DWORD NumberOfClusters;

	BOOL (WINAPI * pSetFilePointerEx)(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);

	void LoadSetFilePointerEx();
	bool IsDriveLetterValid(char *Drive);
	void ResetDiskInfo();
	void LoadDriveInformation(char Drive);
	bool IncFilePointerPosition(LARGE_INTEGER DistanceToMove);
public:
	bool _INTERNAL_LINKAGE IsOpened();
	bool _INTERNAL_LINKAGE Open(char Drive, bool WriteAccess = false);
	void _INTERNAL_LINKAGE Close();

	UI64 _INTERNAL_LINKAGE Tell();
	void _INTERNAL_LINKAGE Seek(LARGE_INTEGER liDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN);
	void _INTERNAL_LINKAGE Seek(UI64 uiDistanceToMove, DWORD dwMoveMethod = FILE_BEGIN);

	DWORD _INTERNAL_LINKAGE ReadBuffer(BYTE *pBuffer, UINT NumBytesToRead);
	DWORD _INTERNAL_LINKAGE ReadSectors(BYTE *pBuffer, UINT NumSectorsToRead);
	DWORD _INTERNAL_LINKAGE WriteBuffer(BYTE *pBuffer, UINT NumBytesToWrite);
	DWORD _INTERNAL_LINKAGE WriteSectors(BYTE *pBuffer, UINT NumSectorsToWrite);

	char _INTERNAL_LINKAGE GetOpenedDriveLetter();

	UI64 _INTERNAL_LINKAGE GetSize();
	UI64 _INTERNAL_LINKAGE GetSectorsCount();

	DWORD _INTERNAL_LINKAGE GetNumberOfClusters();
	DWORD _INTERNAL_LINKAGE GetNumBytesPerSector();
	DWORD _INTERNAL_LINKAGE GetNumSectorsPerCluster();

	UI64 _INTERNAL_LINKAGE GetNumBytesFree(char Drive);
	UI64 _INTERNAL_LINKAGE GetNumBytesUsed(char Drive);
	UI64 _INTERNAL_LINKAGE GetNumSectorFree(char Drive);
	UI64 _INTERNAL_LINKAGE GetNumSectorUsed(char Drive);

#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_CDRIVEIO_H_/*