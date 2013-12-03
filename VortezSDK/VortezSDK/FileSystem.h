#ifndef _CFILESYSTEM_H_
#define _CFILESYSTEM_H_
#ifdef __cplusplus

#ifdef COMPILE_FOR_BORLAND
#pragma comment(lib, "Winmm.lib")
#endif

#include "Preproc.h"

#include "Windows.h"
#include "stdio.h"
#include "Bits.h"
#include "Conversion.h"
#include <direct.h>

#ifdef COMPILE_FOR_BORLAND
struct CFileAttribs {
	bool ReadOnly;
	bool Hidden;
	bool System;
	bool Directory;
	bool Archive;
	bool Normal;
	bool Temporary;
	bool SparseFile;
	bool ReparsePoint;
	bool Compressed;
	bool Offline;
	bool NotIndexed;
	bool Encrypted;
	bool IsVirtual;
};
#endif 

#ifdef COMPILE_FOR_BORLAND
class AFileManager {
public:
	virtual bool _INTERNAL_LINKAGE DoesExist(char *pFileName) = 0;
	virtual void _INTERNAL_LINKAGE Copy(char *pSourceFileName, char *pDestFileName, bool FailIfExist) = 0;
	virtual void _INTERNAL_LINKAGE Move(char *pSourceFileName, char *pDestFileName) = 0;
	virtual void _INTERNAL_LINKAGE Delete(char *pFileName) = 0;
	virtual void _INTERNAL_LINKAGE Rename(char *pOldFileName, char *pNewFileName) = 0;
	virtual int  _INTERNAL_LINKAGE SendToRecycleBin(char *pFileName, WORD Flags = 0x0054) = 0;
	virtual bool _INTERNAL_LINKAGE IsInUse(char *pFileName) = 0;
	virtual UINT _INTERNAL_LINKAGE GetSize(char *pFileName) = 0;
	virtual UI64 _INTERNAL_LINKAGE GetSize64(char *pFileName) = 0;
	virtual void _INTERNAL_LINKAGE GetShortName(char *pLongFileName, char *pShortFileName) = 0;
	virtual UINT _INTERNAL_LINKAGE GetAttributes(char *pFileName, CFileAttribs *pFileAttribs) = 0;
	virtual bool _INTERNAL_LINKAGE SetAttributes(char *pFileName, CFileAttribs *pFileAttribs) = 0;
	virtual bool _INTERNAL_LINKAGE SetAttributes32(char *pFileName, UINT Attribs) = 0;
	virtual void _INTERNAL_LINKAGE Open(char *pFileName, char *pParams) = 0;
	virtual void _INTERNAL_LINKAGE OpenURL(char *pURL) = 0;
	
	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CFileManager : public AFileManager {
#else
class _EXP_FUNC CFileManager {
#endif
#ifndef COMPILE_FOR_BORLAND
public:
	struct CFileAttribs {
		bool ReadOnly;
		bool Hidden;
		bool System;
		bool Directory;
		bool Archive;
		bool Normal;
		bool Temporary;
		bool SparseFile;
		bool ReparsePoint;
		bool Compressed;
		bool Offline;
		bool NotIndexed;
		bool Encrypted;
		bool IsVirtual;
	};
#endif
public:
	bool _INTERNAL_LINKAGE DoesExist(char *pFileName);
	void _INTERNAL_LINKAGE Copy(char *pSourceFileName, char *pDestFileName, bool FailIfExist);
	void _INTERNAL_LINKAGE Move(char *pSourceFileName, char *pDestFileName);
	void _INTERNAL_LINKAGE Delete(char *pFileName);
	void _INTERNAL_LINKAGE Rename(char *pOldFileName, char *pNewFileName);
	int  _INTERNAL_LINKAGE SendToRecycleBin(char *pFileName, WORD Flags = 0x0054);
	bool _INTERNAL_LINKAGE IsInUse(char *pFileName);
	UINT _INTERNAL_LINKAGE GetSize(char *pFileName);
	UI64 _INTERNAL_LINKAGE GetSize64(char *pFileName);
	void _INTERNAL_LINKAGE GetShortName(char *pLongFileName, char *pShortFileName);
	UINT _INTERNAL_LINKAGE GetAttributes(char *pFileName, CFileAttribs *pFileAttribs);
	bool _INTERNAL_LINKAGE SetAttributes(char *pFileName, CFileAttribs *pFileAttribs);
#ifndef COMPILE_FOR_BORLAND
	bool _INTERNAL_LINKAGE SetAttributes(char *pFileName, UINT Attribs);
#else
	bool _INTERNAL_LINKAGE SetAttributes32(char *pFileName, UINT Attribs);
#endif
	void _INTERNAL_LINKAGE Open(char *pFileName, char *pParams);
	void _INTERNAL_LINKAGE OpenURL(char *pURL);
	//void _INTERNAL_LINKAGE GetCRC();
#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#ifdef COMPILE_FOR_BORLAND
class AFolderManager {
public:
	virtual bool _INTERNAL_LINKAGE Create(char *pFolderName) = 0;
	virtual bool _INTERNAL_LINKAGE CreateTree(char *pFolderName) = 0;
	virtual bool _INTERNAL_LINKAGE Delete(char *pFolderName) = 0;
	virtual bool _INTERNAL_LINKAGE DeleteTree(char *pFolderName, UINT Level = 0) = 0;
	virtual void _INTERNAL_LINKAGE Rename(char *pOldName, char *pNewName) = 0;
	virtual void _INTERNAL_LINKAGE SendToRecycleBin(char *pFileName) = 0;
	virtual bool _INTERNAL_LINKAGE DoesExist(char *pFolderName, bool TreatDriveRootAsFolder = true) = 0;
	virtual void _INTERNAL_LINKAGE GetCurrent(char *pFolderName) = 0;
	virtual void _INTERNAL_LINKAGE SetCurrent(char *pFolderName) = 0;
	virtual void _INTERNAL_LINKAGE GetShortName(char *pLongFileName, char *pShortFileName) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CFolderManager : public AFolderManager {
#else
class _EXP_FUNC CFolderManager {
#endif
private:
	void MergePath(char *pDest, char *p1, char *p2);
public:
	bool _INTERNAL_LINKAGE Create(char *pFolderName);
	bool _INTERNAL_LINKAGE CreateTree(char *pFolderName);
	bool _INTERNAL_LINKAGE Delete(char *pFolderName);
	bool _INTERNAL_LINKAGE DeleteTree(char *pFolderName, UINT Level = 0);
	void _INTERNAL_LINKAGE Rename(char *pOldName, char *pNewName);
	void _INTERNAL_LINKAGE SendToRecycleBin(char *pFileName);
	bool _INTERNAL_LINKAGE DoesExist(char *pFolderName, bool TreatDriveRootAsFolder = true);
	void _INTERNAL_LINKAGE GetCurrent(char *pFolderName);
	void _INTERNAL_LINKAGE SetCurrent(char *pFolderName);
	void _INTERNAL_LINKAGE GetShortName(char *pLongFileName, char *pShortFileName);
	//void _INTERNAL_LINKAGE Find();
#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#ifdef COMPILE_FOR_BORLAND
class ADriveManager {
public:
	virtual bool _INTERNAL_LINKAGE DoesExist(char Drive) = 0;
	virtual UINT _INTERNAL_LINKAGE GetAvailableDrives() = 0;
	//virtual void _INTERNAL_LINKAGE GetAvailableDrives(bool *pDrives) = 0;

	virtual UI64 _INTERNAL_LINKAGE FreeSpace(char Drive) = 0;
	virtual UI64 _INTERNAL_LINKAGE UsedSpace(char Drive) = 0;
	virtual UI64 _INTERNAL_LINKAGE GetSize(char Drive) = 0;
	virtual UI64 _INTERNAL_LINKAGE GetNumSectors(char Drive) = 0;
	virtual UINT _INTERNAL_LINKAGE GetNumBytesPerSector(char Drive) = 0;
	
	virtual UINT _INTERNAL_LINKAGE GetType(char Drive) = 0;
	virtual bool _INTERNAL_LINKAGE IsCDRom(char Drive) = 0;
	virtual bool _INTERNAL_LINKAGE IsHardDriveDevice(char Drive) = 0;
	virtual bool _INTERNAL_LINKAGE IsFlashDriveDevice(char Drive) = 0;
	virtual bool _INTERNAL_LINKAGE IsDriveRemovable(char Drive) = 0;
	virtual bool _INTERNAL_LINKAGE IsRemoteDrive(char Drive) = 0;
	virtual bool _INTERNAL_LINKAGE IsRamDisk(char Drive) = 0;
	
	virtual bool _INTERNAL_LINKAGE Information(char Drive, char *pLabel, char *pSerialNumber, char *pFileSystem) = 0;

	virtual void _INTERNAL_LINKAGE OpenCDRom(char Drive, BOOL bOpenDrive) = 0;

	virtual void _INTERNAL_LINKAGE Free() = 0;
};
#endif

#ifdef COMPILE_FOR_BORLAND
class CDriveManager : public ADriveManager {
#else
class _EXP_FUNC CDriveManager {
#endif
private:
	UI64 GetClustersSizeTaken(DWORD NumBytesPerSectors, DWORD NumSectorsPerCluster, DWORD NumClusters);
	bool IsDriveLetterValid(char *Drive);
public:
	bool _INTERNAL_LINKAGE DoesExist(char Drive);
	UINT _INTERNAL_LINKAGE GetAvailableDrives();
	void _INTERNAL_LINKAGE GetAvailableDrives(bool *pDrives);

	UI64 _INTERNAL_LINKAGE FreeSpace(char Drive);
	UI64 _INTERNAL_LINKAGE UsedSpace(char Drive);
	UI64 _INTERNAL_LINKAGE GetSize(char Drive);
	UI64 _INTERNAL_LINKAGE GetNumSectors(char Drive);
	UINT _INTERNAL_LINKAGE GetNumBytesPerSector(char Drive);
	
	UINT _INTERNAL_LINKAGE GetType(char Drive);
	bool _INTERNAL_LINKAGE IsCDRom(char Drive);
	bool _INTERNAL_LINKAGE IsHardDriveDevice(char Drive);
	bool _INTERNAL_LINKAGE IsFlashDriveDevice(char Drive);
	bool _INTERNAL_LINKAGE IsDriveRemovable(char Drive);
	bool _INTERNAL_LINKAGE IsRemoteDrive(char Drive);
	bool _INTERNAL_LINKAGE IsRamDisk(char Drive);
	
	bool _INTERNAL_LINKAGE Information(char Drive, char *pLabel, char *pSerialNumber, char *pFileSystem);

	void _INTERNAL_LINKAGE OpenCDRom(char Drive, BOOL bOpenDrive);
#ifdef COMPILE_FOR_BORLAND
	void _INTERNAL_LINKAGE Free(){if(this) delete this;}
#endif
};

#endif
#endif //_CFILESYSTEM_H_/*