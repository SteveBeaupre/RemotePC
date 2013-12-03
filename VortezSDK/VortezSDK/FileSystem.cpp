#include "FileSystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

bool CFileManager::DoesExist(char *pFileName)
{
	bool Res = false;

	char Buf[MAX_PATH];
	ZeroMemory(Buf, MAX_PATH);
	sprintf(Buf, "%s", pFileName);
    
	WIN32_FIND_DATA Data;
    HANDLE h = FindFirstFile(Buf, &Data);
	if(h != INVALID_HANDLE_VALUE){
		FindClose(h);
		Res = !ReadBit(&Data.dwFileAttributes, 4);
	}

	return Res;
}

void CFileManager::Copy(char *pSourceFileName, char *pDestFileName, bool FailIfExist)
{
	CopyFile(pSourceFileName, pDestFileName, FailIfExist);
}

void CFileManager::Move(char *pSourceFileName, char *pDestFileName)
{
	MoveFile(pSourceFileName, pDestFileName);
}

void CFileManager::Delete(char *pFileName)
{
	DeleteFile(pFileName);
}

void CFileManager::Rename(char *pOldFileName, char *pNewFileName)
{
	rename(pOldFileName, pNewFileName);
}

int CFileManager::SendToRecycleBin(char *pFileName, WORD Flags)
{
	SHFILEOPSTRUCT fo;
	fo.hwnd = NULL;
	fo.wFunc = FO_DELETE;
	fo.pFrom = pFileName;
	fo.pTo = NULL;
	fo.fFlags = Flags;
	fo.fAnyOperationsAborted = FALSE;
	fo.hNameMappings = NULL;
	fo.lpszProgressTitle = NULL;
	
	return SHFileOperation(&fo);
}

bool CFileManager::IsInUse(char *pFileName)
{
	if(!DoesExist(pFileName))
		return false;

	HANDLE f = CreateFile(pFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	bool Res = (f == INVALID_HANDLE_VALUE);
	if(f != INVALID_HANDLE_VALUE)
		CloseHandle(f);
	return Res;
}

UINT CFileManager::GetSize(char *pFileName)
{
	char Buf[MAX_PATH];
	ZeroMemory(Buf, sizeof(Buf));
	sprintf(Buf, "%s", pFileName);
    
	WIN32_FIND_DATA Data;
 	ZeroMemory(&Data, sizeof(WIN32_FIND_DATA));
    HANDLE h = FindFirstFile(Buf, &Data);
	if(h != INVALID_HANDLE_VALUE)
		FindClose(h);

	return Data.nFileSizeLow;
}

UI64 CFileManager::GetSize64(char *pFileName)
{
	char Buf[MAX_PATH];
	ZeroMemory(Buf, sizeof(Buf));
	sprintf(Buf, "%s", pFileName);
    
	WIN32_FIND_DATA Data;
	ZeroMemory(&Data, sizeof(WIN32_FIND_DATA));
    HANDLE h = FindFirstFile(Buf, &Data);
	if(h != INVALID_HANDLE_VALUE)
		FindClose(h);

	return DWORDX2ToUI64(Data.nFileSizeHigh, Data.nFileSizeLow);
}

void CFileManager::GetShortName(char *pLongFileName, char *pShortFileName)
{
	GetShortPathName(pLongFileName, pShortFileName, MAX_PATH);
}

UINT CFileManager::GetAttributes(char *pFileName, CFileAttribs *pFileAttribs)
{
	UINT Attribs = GetFileAttributes(pFileName);

	if(pFileAttribs != NULL){
		pFileAttribs->ReadOnly     = ReadBit(&Attribs, 0);
		pFileAttribs->Hidden       = ReadBit(&Attribs, 1);
		pFileAttribs->System       = ReadBit(&Attribs, 2);
		pFileAttribs->Directory    = ReadBit(&Attribs, 4);
		pFileAttribs->Archive      = ReadBit(&Attribs, 5);
		pFileAttribs->Normal       = ReadBit(&Attribs, 7);
		pFileAttribs->Temporary    = ReadBit(&Attribs, 8);
		pFileAttribs->SparseFile   = ReadBit(&Attribs, 9);
		pFileAttribs->ReparsePoint = ReadBit(&Attribs, 10);
		pFileAttribs->Compressed   = ReadBit(&Attribs, 11);
		pFileAttribs->Offline      = ReadBit(&Attribs, 12);
		pFileAttribs->NotIndexed   = ReadBit(&Attribs, 13);
		pFileAttribs->Encrypted    = ReadBit(&Attribs, 14);
		pFileAttribs->IsVirtual    = ReadBit(&Attribs, 16);
	}

	return Attribs;
}

bool CFileManager::SetAttributes(char *pFileName, CFileAttribs *pFileAttribs)
{
	if(pFileAttribs == NULL)
		return false;

	UINT Attribs = 0;
	WriteBit(&Attribs, 0,  pFileAttribs->ReadOnly);
	WriteBit(&Attribs, 1,  pFileAttribs->Hidden);
	WriteBit(&Attribs, 2,  pFileAttribs->System);
	WriteBit(&Attribs, 4,  pFileAttribs->Directory);
	WriteBit(&Attribs, 5,  pFileAttribs->Archive);
	WriteBit(&Attribs, 7,  pFileAttribs->Normal);
	WriteBit(&Attribs, 8,  pFileAttribs->Temporary);
	WriteBit(&Attribs, 9,  pFileAttribs->SparseFile);
	WriteBit(&Attribs, 10, pFileAttribs->ReparsePoint);
	WriteBit(&Attribs, 11, pFileAttribs->Compressed);
	WriteBit(&Attribs, 12, pFileAttribs->Offline);
	WriteBit(&Attribs, 13, pFileAttribs->NotIndexed);
	WriteBit(&Attribs, 14, pFileAttribs->Encrypted);
	WriteBit(&Attribs, 16, pFileAttribs->IsVirtual);

#ifndef COMPILE_FOR_BORLAND
	return SetAttributes(pFileName, Attribs);
#else
	return SetAttributes32(pFileName, Attribs);
#endif
}

#ifndef COMPILE_FOR_BORLAND
bool CFileManager::SetAttributes(char *pFileName, UINT Attribs)
#else
bool CFileManager::SetAttributes32(char *pFileName, UINT Attribs)
#endif
{
	return SetFileAttributes(pFileName, Attribs) != FALSE;
}

void CFileManager::Open(char *pFileName, char *pParams)
{
	ShellExecute(0, "open", pFileName, pParams, 0, SW_SHOWDEFAULT);
}

void CFileManager::OpenURL(char *pURL)
{
	ShellExecute(0, "open", pURL, 0, 0, SW_SHOWDEFAULT);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void CFolderManager::MergePath(char *pDest, char *p1, char *p2)
{
	if(strlen(p1) > 0 && p1[strlen(p1)-1] != '\\'){
		strcat(pDest, p1);
		strcat(pDest, "\\");
		strcat(pDest, p2);
	} else {
		strcat(pDest, p1);
		strcat(pDest, p2);
	}
}

bool IsPathDriveRoot(char *path)
{
	int Len = strlen(path);
	if(Len == 2 || Len == 3){
		if((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z')){	
			if(path[1] == ':'){
				if(Len == 2){
					return true;			
				} else {
					if(path[2] == '\\')
						return true;
				}
			}
		}
	}

	return false;
}

bool CFolderManager::DoesExist(char *pFolderName, bool TreatDriveRootAsFolder)
{
	if(TreatDriveRootAsFolder && IsPathDriveRoot(pFolderName)){
		CDriveManager DriveManager;
		return DriveManager.DoesExist(pFolderName[0]);
	}

	bool Res = false;

	char Buf[MAX_PATH];
	ZeroMemory(Buf, sizeof(Buf));

	MergePath(Buf, pFolderName, "*");

	WIN32_FIND_DATA Data;
    HANDLE h = FindFirstFile(Buf, &Data);
	if(h != INVALID_HANDLE_VALUE){
		FindClose(h);
		Res = ReadBit(&Data.dwFileAttributes, 4);
	}

	return Res;
}

bool CFolderManager::Create(char *pFolderName)
{
	return _mkdir(pFolderName) != -1;
}

bool CFolderManager::CreateTree(char *pFolderName)
{
	char Buf[MAX_PATH];
	ZeroMemory(Buf, sizeof(Buf));

	int Len = strlen(pFolderName);

	for(int Cpt = 0; Cpt < Len; Cpt++){
	
		Buf[Cpt] = pFolderName[Cpt];
		
		if(Buf[Cpt] == '\\'){
			if((Cpt == Len-1) || (Cpt > 0 && Buf[Cpt-1] == ':'))
				continue;

			if(!this->DoesExist(Buf)){
				if(!this->Create(Buf))
					return false;
			}
		}
	}

	return this->Create(Buf);
}

bool CFolderManager::Delete(char *pFolderName)
{
	return _rmdir(pFolderName) != -1;
}

bool CFolderManager::DeleteTree(char *pFolderName, UINT Level)
{
	// Append a "\*" or "*" to the end of the current path
	char path[MAX_PATH];
	ZeroMemory(&path[0], MAX_PATH);
	MergePath(path, pFolderName, "*");

	WIN32_FIND_DATA fData;
	// If the directory is valid...
	if(this->DoesExist(pFolderName)){
		
		HANDLE h = FindFirstFile(&path[0], &fData);

		// Add a sub-directories to the end of the current node childs list
		do{	
			if(strcmp(fData.cFileName, ".") != 0 && strcmp(fData.cFileName, "..") != 0){
				char newpath[MAX_PATH];
				ZeroMemory(&newpath[0], MAX_PATH);
				MergePath(&newpath[0], pFolderName, fData.cFileName);

				// Check if this is a file or directory
				switch(ReadBit(&fData.dwFileAttributes, 4))
				{
				case false: DeleteFile(&newpath[0]);          break;
				case true:  DeleteTree(&newpath[0], Level+1); break;
				}
			}
		} while(FindNextFile(h, &fData) != NULL);

		// Close the handle
		FindClose(h);
	}

	// Now that the folder is empty, we can delete it
	this->Delete(pFolderName);

	return this->DoesExist(pFolderName);
}

void CFolderManager::Rename(char *pOldName, char *pNewName)
{
	rename(pOldName, pNewName);
}

void CFolderManager::SendToRecycleBin(char *pFileName)
{
	CFileManager FileManager;
	FileManager.SendToRecycleBin(pFileName);
}

void CFolderManager::GetCurrent(char *pFolderName)
{
	_getcwd(pFolderName, MAX_PATH);
}


void CFolderManager::SetCurrent(char *pFolderName)
{
	#ifndef RAD_STUDIO_XE
	_chdir(pFolderName);
	#else
	chdir(pFolderName);
	#endif
}

void CFolderManager::GetShortName(char *pLongFileName, char *pShortFileName)
{
	GetShortPathName(pLongFileName, pShortFileName, MAX_PATH);
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
UI64 CDriveManager::GetClustersSizeTaken(DWORD NumBytesPerSectors, DWORD NumSectorsPerCluster, DWORD NumClusters)
{
	UI64 Res = NumBytesPerSectors;
	Res *= NumSectorsPerCluster;
	Res *= NumClusters;
	return Res;
}

bool CDriveManager::DoesExist(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return false;}

	UINT DrivesMask = GetLogicalDrives();

	return ReadBit(&DrivesMask, Drive-65);
}

UINT CDriveManager::GetAvailableDrives()
{
	return GetLogicalDrives();
}

void CDriveManager::GetAvailableDrives(bool *pDrives)
{
	UINT DrivesMask = GetLogicalDrives();
	for(int Cpt = 0; Cpt < 26; Cpt++)
		pDrives[Cpt] = ReadBit(&DrivesMask, Cpt);
}



UI64 CDriveManager::FreeSpace(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return 0;}
	char szRoot[4] = {Drive,':', '\\', 0};
	
	DWORD NumSectorsPerCluster, NumBytesPerSectors, NumFreeClusters, TotalClusterCount;
	GetDiskFreeSpace(&szRoot[0], &NumSectorsPerCluster, &NumBytesPerSectors, &NumFreeClusters, &TotalClusterCount);
	return GetClustersSizeTaken(NumBytesPerSectors, NumSectorsPerCluster, NumFreeClusters);
}


UI64 CDriveManager::UsedSpace(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return 0;}

	return GetSize(Drive) - FreeSpace(Drive);
}


UI64 CDriveManager::GetSize(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return 0;}
	char szRoot[4] = {Drive,':', '\\', 0};
	
	DWORD NumSectorsPerCluster, NumBytesPerSectors, NumFreeClusters, TotalClustersCount;
	GetDiskFreeSpace(&szRoot[0], &NumSectorsPerCluster, &NumBytesPerSectors, &NumFreeClusters, &TotalClustersCount);

	return GetClustersSizeTaken(NumBytesPerSectors, NumSectorsPerCluster, TotalClustersCount);
}

UI64 CDriveManager::GetNumSectors(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return 0;}
	char szRoot[4] = {Drive,':', '\\', 0};
	
	DWORD NumSectorsPerCluster, NumBytesPerSectors, NumFreeClusters, TotalClustersCount;
	GetDiskFreeSpace(&szRoot[0], &NumSectorsPerCluster, &NumBytesPerSectors, &NumFreeClusters, &TotalClustersCount);

	UI64 NumSectors = NumSectorsPerCluster;
	NumSectors *= TotalClustersCount;
	return NumSectors;
}

UINT CDriveManager::GetNumBytesPerSector(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return 0;}
	char szRoot[4] = {Drive,':', '\\', 0};
	
	DWORD NumSectorsPerCluster, NumBytesPerSectors, NumFreeClusters, TotalClustersCount;
	GetDiskFreeSpace(&szRoot[0], &NumSectorsPerCluster, &NumBytesPerSectors, &NumFreeClusters, &TotalClustersCount);

	return NumBytesPerSectors * NumSectorsPerCluster;
}
	
UINT CDriveManager::GetType(char Drive)
{
	if(!IsDriveLetterValid(&Drive)){return (UINT)(-1);}
	char szRoot[4] = {Drive,':', '\\', 0};
	
	return GetDriveType(&szRoot[0]);
}

bool CDriveManager::IsCDRom(char Drive)
{
	return this->GetType(Drive) == DRIVE_CDROM;
}

bool CDriveManager::IsHardDriveDevice(char Drive)
{
	return this->GetType(Drive) == DRIVE_FIXED;
}

bool CDriveManager::IsFlashDriveDevice(char Drive)
{
	return this->GetType(Drive) == DRIVE_FIXED;
}

bool CDriveManager::IsDriveRemovable(char Drive)
{
	return this->GetType(Drive) == DRIVE_REMOVABLE;
}

bool CDriveManager::IsRemoteDrive(char Drive)
{
	return this->GetType(Drive) == DRIVE_REMOTE;
}

bool CDriveManager::IsRamDisk(char Drive)
{
	return this->GetType(Drive) == DRIVE_RAMDISK;
}


bool CDriveManager::Information(char Drive, char *pLabel, char *pSerialNumber, char *pFileSystem)
{
	bool Res = false;

	if(!IsDriveLetterValid(&Drive)){return Res;}
	char szRoot[4] = {Drive,':', '\\', 0};

	char  VolumeName[MAX_PATH];
	DWORD VolumeSerialNumber;
	char  FileSystemName[MAX_PATH];

	Res = GetVolumeInformation(&szRoot[0], &VolumeName[0], MAX_PATH, &VolumeSerialNumber, NULL, NULL, &FileSystemName[0], MAX_PATH) == TRUE;
	
	if(pLabel)
		strcpy(pLabel, &VolumeName[0]);
	if(pSerialNumber)
		#ifndef RAD_STUDIO_XE
		_itoa(VolumeSerialNumber, pSerialNumber, 10);
		#else
		itoa(VolumeSerialNumber, pSerialNumber, 10);
		#endif

	if(pFileSystem)
		strcpy(pFileSystem, &FileSystemName[0]);

	return Res;
}

void CDriveManager::OpenCDRom(char Drive, BOOL bOpenDrive)
{ 
	if(!IsDriveLetterValid(&Drive) || !IsCDRom(Drive)){return;}
	char szRoot[4] = {Drive,':', '\\', 0};

    MCI_OPEN_PARMS open;
    DWORD flags;

    ZeroMemory(&open, sizeof(MCI_OPEN_PARMS));

    open.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_CD_AUDIO;
    open.lpstrElementName = &szRoot[0];

    flags = MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID;

    if(!mciSendCommand(0, MCI_OPEN, flags, (DWORD) &open)){
        mciSendCommand(open.wDeviceID, MCI_SET, (bOpenDrive) ? MCI_SET_DOOR_OPEN : MCI_SET_DOOR_CLOSED, 0);
        mciSendCommand(open.wDeviceID, MCI_CLOSE, MCI_WAIT, 0);
    }
}

bool CDriveManager::IsDriveLetterValid(char *Drive)
{
	if(*Drive >= 'a' && *Drive <= 'z'){
		*Drive -= 32;
		return true;
	} else if(*Drive >= 'A' && *Drive <= 'Z'){
		return true;
	}

	return false;
}


